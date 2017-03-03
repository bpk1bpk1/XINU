/* future.c - future_alloc future_free future_get future_set */

#include <xinu.h>
#include <future.h>


/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */


future_t* future_alloc(future_mode_t mode) {

  // ensure argument is valid
  if ( (mode != FUTURE_EXCLUSIVE) && (mode != FUTURE_SHARED) && (mode != FUTURE_QUEUE) ) {
    return NULL;
  }

  // allocate space for our future
  future_t* fut = (future_t*) getmem(sizeof(future_t));

  // ensure getmem succeeded
  if ((char *) fut == (char *) SYSERR) {
    return NULL;
  }

  // initialize future values
  fut->value = 0;
  fut->state = FUTURE_EMPTY;
  fut->mode = mode;
  fut->pid =  getpid();
  fut->set_queue = NULL;
  fut->get_queue = NULL;

  return fut;
}

syscall future_free(future_t* fut) {

  kprintf("freeing future");

  return freemem((char*) fut, sizeof(future_t));

}

syscall future_get(future_t* fut, int* dst) {

  if (fut->state != FUTURE_READY) {
    fut->state = FUTURE_WAITING;
    fut->pid = getpid();
    suspend(getpid());
  }
    
  *dst = fut->value;
  fut->state = FUTURE_EMPTY;

  return OK;
}

syscall future_set(future_t* fut, int src) {

  fut->value = src;

  if (fut->state == FUTURE_WAITING) {
    resume(fut->pid);
  }

  fut->state = FUTURE_READY;

  return OK;
}
