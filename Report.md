# Assignment 2 - Robert Henderson and Burde Prerana Kamath

This assignment provided an overview as to how processes run concurrently, why this is so important, and provided hand-on experience writing concurrent code.

## 1 - Why does your program hang? What is the name of this behavior in an operating system? 

The name of this behaviour is called deadlock. 

Deadlock was originally achieved by replacing variable 2 with variable 1. When every loop begins, variable 2 and variable 1 of the process is the same. The next step in the program is to assign variable 2 to variable 1 of the process currently running. If the processes were synchronised, there wouldnt be a problem. The program hangs because in an asynchronous behaviour, variable 2 is decremented before a system call is made due to the printf statement. Hence, this variable2 assigns the same value to variable 1 and the entry into the if statement becomes void. This applies to all the processes in the program and they keep checking whether the if condition is going to be true. The term for this is called Spinlock or "Busy Waiting" in popular terms. 

This feature was implemented by wrapping the initalizing signal(sid32) in an if statement. This leaves all the processes waiting patiently for a signal that will never come.

## 2 - Why does your program go into an infinite loop? What is the name of this behavior in an operating system?

The name of this behaviour is infinite loop.

The first process running is going to be interrupted by the Input/Output interrupt before it can finish its execution. The value is decremented by the number of processes being run. After all the processes are run, variable2 is going to be decremented but the value cannot be written to variable 1 of the next process as a context switch occurs before that. When the next process runs, it uses the same value in its variable 1 and variable 2 ends up getting decremented to the same value again. As this keeps repeating, the variable 2 doesn't reach the value 0 and the processes never seem to exit the loop.

This feature was implemented by wrapping the return-statement of the while-loop in an if-statement. This prevented the loop from stopping when the count hit zero and let it continue deep into the negatives.

## 3 - Why does your program print numbers out of order? What is the name of this behavior in an operating system?

This behaviour is popularly known as a Race condition.

To set our race condition up, a chaotic copy of process_ring was created without any of the semophores to cooridate the concurrently running process. Each process was free to calculate and print it's rows of the output at it's own pace. A short sleep was added to ensure that the outputs of our 4 processes would overlap and be different every time the program was ran.

## 4 - What property of Xinu allows the working version of your program to print values in the correct order?

The current implementation relied on semaphores to manage the concurrent processes. Each process would wait in line on a single semophore to perform it's task and then signal the next process to proceed once it had done it's job of printing and incrementing the count variable. A sleep(0) statement gave the signalling process time to get back in line before the next round began.

The original implementation used if statements to control how each process would print the Ring element along with the round and the value. The condition for this is the remainder when variable2 is divided by the numberofprocesses. The remainder has to be equal to one of the process id's for the print statement to be executed during which all the other processes are going to be in the waiting state. 

## Contribution of each team member

 - Both Bo and Prerana built skeleton code and part of the command line arguement parser
 - Bo upgraded the command line argument parser to make it more robust
 - Prerana implemented the work, hang, loop, and most of the chaotic functionality as 4 separate functions
 - Preana answered all 4 questions of the report
 - Bo consolidated Prerana's work, hang, and loop code into a single function and wired up the command line arguments
 - Bo finished the out-of-order functionality
 - Bo updated the report to reflect the changes he made.
