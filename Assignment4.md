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


## Markdown tips/tricks

lines that start with # are headers, the more # there are, the smaller the text so "# big header" "### smaller header" "##### very small header"

Here's some pretty code.. use three backticks followed by a language name for syntax highlighting.. c should be the only language we care about though.

```c
#include <xinu.h>
#include <string.h>
#include <stdio.h>

shellcmd xsh_hello(int nargs, char *args[]) {
	if (nargs > 2) {
		printf("%s: Too many arguments\n", args[0]);
		return 1;
	}
	
	if (nargs < 2) {
		printf("Too few arguments \n");
	}

	if (nargs == 2) {
		printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
	}
	
	return 0;
}
```
