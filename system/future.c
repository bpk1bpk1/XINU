/* future.c - future_alloc future_free future_get future_set */

#include <xinu.h>
#include <future.h>


/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
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
  fut->pid =  getpid();
  fut->set_queue = NULL;
  fut->get_queue = NULL;
  return fut;
}

syscall future_free(future_t* fut) {

  node *curr, *next;

  // free the set_queue
  if (fut->set_queue != NULL) {
    curr = fut->set_queue;
    next = curr->next;
    do {
      freemem(curr, sizeof(node));
      curr = next;
      next = curr->next;
    } while (next != NULL);
  }

  // free the get_queue
  if (fut->get_queue != NULL) {
    curr = fut->get_queue;
    next = curr->next;
    do {
      freemem(curr, sizeof(node));
      curr = next;
      next = curr->next;
    } while (next != NULL);
  }

  return freemem((char*) fut, sizeof(future_t));

}

syscall future_get(future_t* fut, int* dst) {

  // Exclusive mode: 1 on 1
  if (fut->mode == FUTURE_EXCLUSIVE) {
    // if not ready, wait until it is
    if (fut->state != FUTURE_READY) {
      fut->state = FUTURE_WAITING;
      fut->pid = getpid();
      suspend(getpid());
    }
  }

  // Shared mode: 1 on many
  else if (fut->mode == FUTURE_SHARED) {
    // if the future isn't ready, then it's waiting
    if (fut->state != FUTURE_READY) {
      fut->state = FUTURE_WAITING;
      // create a node to add to our get_queue
      node* get_node = (node*) getmem(sizeof(node));
      get_node->pid = getpid();
      get_node->next = NULL;
      // if the queue is empty, then our node is the new get_queue
      if (fut->get_queue == NULL) { fut->get_queue = get_node; }
      // otherwise, follow the linked list to the end and add our node there
      else {
	node* curr = fut->get_queue;
	while (curr->next != NULL) {
	  curr = curr->next;
	}
	curr->next = get_node;
      }
      // wait to be called before continuing
      suspend(getpid());
    }
  }

  // Queue mode: many on many
  else if (fut->mode == FUTURE_QUEUE) {


    // queue something


  } else {
    return SYSERR;
  }

  // future is ready, let's empty it!
  *dst = fut->value;
  fut->state = FUTURE_EMPTY;
  return OK;
}

syscall future_set(future_t* fut, int src) {

  // Exclusive mode: 1 on 1
  if (fut->mode == FUTURE_EXCLUSIVE) {
    fut->value = src;
    // if a process is waiting, resume it
    if (fut->state == FUTURE_WAITING) { resume(fut->pid); }
    fut->state = FUTURE_READY;
  }

  // Shared mode: 1 on many
  else if (fut->mode == FUTURE_SHARED) {

    // throw an error if another processes tries to set this future
    if (fut->state == FUTURE_READY && fut->pid != getpid()) { return SYSERR; }

    fut->value = src;

    // 
    if (fut->state == FUTURE_WAITING) {
      node* curr = fut->get_queue;
      while (curr != NULL) {
	resume(curr->pid);
	curr = curr->next;
      }
    }

    fut->state = FUTURE_READY;
  }

  // queue mode: many on many
  else if (fut->mode == FUTURE_QUEUE) {


    // queue something


  } else {
    return SYSERR;
  }
  return OK;
}

