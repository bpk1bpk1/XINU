# Assignment 4
#### By Bo Henderson (rojahend) and Burde Prerana Kamath (bkamath)

<!--
compile with this command:
`pandoc -V geometry:margin=1in -o Assignment4.pdf Assignment4.md` 
-->

## Question 1

> Consider a modification to the code that stores processor state in the process table instead of on the process’s stack (i.e., assume the process table entry contains an array that holds the contents of registers). What are the advantages of each approach? 

Process Table:

If the information is stored in the process table, then the system will only have to push/pop the state information everytime the process changes. This will save time and memory. In addition to this, no stack pointer will be needed. Moreover, pointers are slower and could possibly lead to memoy leakage. Hence, the user of pointers can be avoided too.

Stack:

 - Stack can be used for pushing and popping in the Last in First Out way where the process that has been entered the last would be popped first.
 - It isn't useful for reversing the way the processes will be popped out. 
 - It can be very useful for navigating tree structures. 
 - Comes in handy for remembering a particular process state. If there is another process that modifies the state it can still revert to the previous state
 - Example: CPU registers, Implementing undo/redo behaviour.


## Question 2

> When a process kills itself, kill deallocates the stack and then calls resched, which means the process continues to use the deallocated stack. Redesign the system so a current process does not deallocate its own stack, but instead moves to a new state, PR\_DYING. Arrange for whatever process searches the process table to look for dying processes, free the stack, and move the entry to PR\_FREE. 

## Question 3

> Function resume saves the resumed process’s priority in a local variable before calling ready. Show that if it references prptr->prprio after the call to ready, resume can return a priority value that the resumed process never had (not even after resumption).

```c
/* resume.c - resume */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  resume  -  Unsuspend a process, making it ready
 *------------------------------------------------------------------------
 */
pri16	resume(
	  pid32		pid		/* ID of process to unsuspend	*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent \*prptr;	/* Ptr to process' table entry	*/
	pri16	prio;			/* Priority to return		*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return (pri16)SYSERR;
	}
	prptr = &proctab[pid];
	if (prptr->prstate != PR_SUSP) {
		restore(mask);
		return (pri16)SYSERR;
	}
	prio = prptr->prprio;		/* Record priority to return	*/
	ready(pid);
	restore(mask);
	return prio;
}
```

The important thing to notice is that, in the last few lines there are 2 opportunities for an arbirary number of other functions to execute: 

 1. Ready calls resched() internally
 2. restore(mask) might re-enable interrupts causing another process to start executing. 

Note that at either of these points, another process could call chprio() and change the priority of some process. If we swapped the lines `prio = prptr->prprio` and `ready(pid)` then we're saving our local variable in between these two points in time when the target process's prioritiy might change. As a result, there is a change that this processes's priority might change, this changed priority is saved, and then it's changed again right before the saved value is returned. In this case, the returned priority is neither the priority of the process before it was resumed nor the priority of the process after it was resumed.
