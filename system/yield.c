/* yield.c - yield */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  yield  -  Voluntarily relinquish the CPU (end a timeslice)
 *------------------------------------------------------------------------
 */
syscall	yield(void)
{
	intmask	mask;			/* Saved interrupt mask		*/

	mask = disable();
	resched(-1);
	restore(mask);
	return OK;
}
