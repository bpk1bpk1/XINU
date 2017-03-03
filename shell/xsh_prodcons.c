/*  */

#include <xinu.h>
#include <future.h>

/*------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------
 */
shellcmd xsh_prodcons(int nargs, char *args[]) {

  chprio(getpid(), 19); // this isn't such an important process

  future_t* f_exclusive, * f_shared, * f_queue;

  f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
  f_shared    = future_alloc(FUTURE_SHARED);
  f_queue     = future_alloc(FUTURE_QUEUE);

  // Test FUTURE_EXCLUSIVE
  resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, 1) );
  future_free(f_exclusive);

  // Test FUTURE_SHARED
  resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
  resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
  resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
  resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
  resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, 2) );
  future_free(f_shared);

  // Test FUTURE_QUEUE
  resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
  resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
  resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue, 3) );
  resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue, 4) );
  resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue, 5) );
  resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue, 6) );
  future_free(f_queue);

  return OK;
}
