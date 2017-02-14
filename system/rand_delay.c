/* rand_delay.c - rand_delay */

#include <xinu.h>

uint32 rseed;

/*------------------------------------------------------------------------
 *  rand_delay  -  Delay the calling process my some number of seconds
 *------------------------------------------------------------------------
 */
syscall	rand_delay(
	  uint32	maxdelay		/* Max time to delay in mili seconds	*/
	)
{

  uint32 now;
  uint32 delay;

  // returns an integer that increases every second
  rseed += gettime(&now); 

  // scramble this integer
  delay = ((rseed++   * 12345) % maxdelay);
  delay = ((delay * 56789) % maxdelay);
  delay = ((delay * 76543) % maxdelay);

  // sleep!
  sleepms(delay);
  return OK;
}
