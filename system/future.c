/* future.c - future_alloc future_free future_get future_set */

#include <xinu.h>
#include <future.h>

node* get_in_line(node* queue);
void free_queue(node* queue);

/*------------------------------------------------------------------------
 *  Futures stuff
 *------------------------------------------------------------------------
 */


future_t* future_alloc(future_mode_t mode) {

  // abort if argument is invalid
  if ( (mode != FUTURE_EXCLUSIVE) && (mode != FUTURE_SHARED) && (mode != FUTURE_QUEUE) ) { return NULL; }

  // allocate space for our future
  future_t* fut = (future_t*) getmem(sizeof(future_t));

  // abort if getmem() failed
  if ((char *) fut == (char *) SYSERR) { return NULL; }

  // initialize future values and return
  fut->value = NULL;
  fut->state = FUTURE_EMPTY;
  fut->mode = mode;
  fut->pid =  NULL;
  fut->set_queue = NULL;
  fut->get_queue = NULL;
  return fut;
}


// free memory used by future and it's queues
syscall future_free(future_t* fut) {
  free_queue(fut->get_queue); // deallocate nodes on the get_queue
  free_queue(fut->set_queue); // deallocate nodes on the set_queue
  return freemem((char*) fut, sizeof(future_t)); // deallocate future
}


syscall future_get(future_t* fut, int* dst) {

  intmask mask = disable();
  node* temp;

  //////////////////////////////
  // Exclusive mode: 1 on 1
  if (fut->mode == FUTURE_EXCLUSIVE) {
    // if not ready, wait until it is
    if (fut->state != FUTURE_READY) {
      fut->state = FUTURE_WAITING;
      fut->pid = getpid();
      suspend(getpid());
      fut->pid = NULL;
    }
    fut->state = FUTURE_EMPTY;
  }
  //////////////////////////////
  // Shared mode: 1 on many
  else if (fut->mode == FUTURE_SHARED) {
    // if the future isn't ready, then we need to be patient
    if (fut->state != FUTURE_READY) {
      fut->state = FUTURE_WAITING;
      fut->get_queue = get_in_line(fut->get_queue);
      suspend(getpid());
    } // note: state is not set to empty for this mode
  }
  //////////////////////////////
  // Queue mode: many on many
  else if (fut->mode == FUTURE_QUEUE) {
    // if we're not ready to get, then wait in line
    if (fut->state != FUTURE_READY) {
      fut->state = FUTURE_WAITING;
      fut->get_queue = get_in_line(fut->get_queue);
      suspend(getpid());
    } 
    // if we're ready to get, then let one setter set
    else {
      if (fut->set_queue != NULL) {
	// resume the first node of the set queue
	temp = fut->set_queue;
	fut->set_queue = fut->set_queue->next;
	resume(temp->pid);
	// free the node that was on the set_queue
	freemem((char*) temp, sizeof(node));
      }
    }
    fut->state = FUTURE_EMPTY;

  } else { restore(mask); return SYSERR; } // unrecognized mode

  *dst = fut->value; // every single future_get ends by saving fut->value in dst
  restore(mask); return OK;
}

syscall future_set(future_t* fut, int src) {

  intmask mask = disable();
  node* temp;

  //////////////////////////////
  // Exclusive mode: 1 on 1
  if (fut->mode == FUTURE_EXCLUSIVE) {
    fut->value = src;
    fut->state = FUTURE_READY;
    if (fut->pid != NULL) { resume(fut->pid); }
  }
  //////////////////////////////
  // Shared mode: 1 on many
  else if (fut->mode == FUTURE_SHARED) {
    // throw an error if another processes tries to set this future
    if (fut->state == FUTURE_READY && fut->pid != getpid()) { 
      restore(mask); return SYSERR; 
    }
    // everyone in the get_queue can go now
    fut->value = src;
    if (fut->state == FUTURE_WAITING) {
      fut->state = FUTURE_READY;
      temp = fut->get_queue;
      while (temp != NULL) {
	resume(temp->pid);
	temp = temp->next;
      }
    }
  }
  //////////////////////////////
  // queue mode: many on many
  else if (fut->mode == FUTURE_QUEUE) {
    if (fut->state != FUTURE_READY) {
      // fill in an empty/waiting future
      fut->value = src;
      fut->state = FUTURE_READY;
      resume(fut->get_queue->pid);
      // free the node that was on the get_queue
      temp = fut->get_queue;
      fut->get_queue = fut->get_queue->next;
      freemem((char*) temp, sizeof(node));
    }
    // wait if this future is already ready
    else {
      fut->set_queue = get_in_line(fut->set_queue);
      suspend(getpid());
      fut->value = src;
      fut->state = FUTURE_READY;
    }

  } else { restore(mask); return SYSERR; } // unrecognized mode

  restore(mask); return OK;
}


// adds the current process's PID to the given queue
node* get_in_line(node* queue) {

  // create a new node to put in the queue
  node* new_node = (node*) getmem(sizeof(node));
  new_node->pid = getpid();
  new_node->next = NULL;

  // if the queue is empty, then our node is the new get_queue
  if (queue == NULL) { return new_node; }

  // otherwise, follow the linked list to the end and add our node there
  else {
    node* curr = queue;
    // go to the end of the line
    while (curr->next != NULL) { curr = curr->next; }
    curr->next = new_node;
  }
  return queue;
  
}

// frees the given node and every node it links to
void free_queue(node* queue) {

  // if it's already cleared then there's nothing for us to do..
  if (queue == NULL) { return; }

  // save the next node and deallocate the current one
  node* next = queue->next;
  freemem((char*) queue, sizeof(node));

  // check, shift, delete, repeat
  while (next != NULL) {
    queue = next;
    next = queue->next;
    freemem((char*) queue, sizeof(node));
  }
}
