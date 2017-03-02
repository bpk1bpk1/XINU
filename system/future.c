/* future.c - future_alloc future_free future_get future_set */

#include <xinu.h>
#include <future.h>


/*------------------------------------------------------------------------
 *  sleep  -  Delay the calling process n seconds
 *------------------------------------------------------------------------
 */


future_t* future_alloc(future_mode_t mode) {
  return 0;
}

syscall future_free(future_t* future) {
  return 0;
}

syscall future_get(future_t* future, int* dst) {
  return 0;
}

syscall future_set(future_t* future, int src) {
  return 0;
}
