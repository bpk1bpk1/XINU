# Assignment 6: Baby Bird Problem

### rojahend & bkamath

Initially, the mother bird process is created and it obtains the parent bird mutex variable parentbird and is waiting on the conditional variable. The baby bird’s processes are also created. The program has two conditional variables – one for the parent and one for the baby birds. The mutex and the conditional variable used in the system calls header were defined in the kernel file. 

Since the number of worms the mother bird has fetched is initially zero, the baby bird is going to acquire the baby bird mutex and signal the conditional variable of the parent. Therefore, baby bird is going to wait on its conditional variable(babybird_cond) and the temporary baby bird mutex till the mother can go and fetch the worms. The mother bird process is going to fill the maximum number of worms set in the command line once it is allowed to run using the conditional mutex, it then signals the baby bird's conditional variable and wait on its conditional variable yet again since the previous value was stored and returned. The baby bird is going to acquire the temporary baby bird mutex(babybird1) because it has already acquired the mutex babybird when it was his/her chance to eat.The baby bird then eats the worm and goes back in queue by unlock the baby bird mutex making it possible for the next bird to get hold of the babybird mutex. This is the way the conditional variables are implemented. For the program to end, every time a baby bird process has eaten its share of worms and it exits, a variable babyEnds is incremented. Once the babyEnds is equivalent to the number of birds, then the bird who eats the last worm signals the mother’s conditional variable. The mother bird’s process checks the condition if all its babies have eaten and since the if condition will be true, the programs breaks from the loop. It prints the number of worms left since it’s a global variable that kept getting decremented. It then exits the program.

The following system files were created in the program (These headers were initialized in the prototypes file in the include folder): 

 1. mutex\_create – This helped created the mutex variables for the program. Babybird, parentbird and babybird1(The temporary baby bird)
 2. mutex\_lock – In this system call, the mutex’s keep checking if the condition in the test-set function is true. This process of checking the conditional variable is called spinning.
 3. mutex\_unlock – This helps unlock the mutex variables.
 4. cond\_init – This helped initialize the conditional variables babybird\_cond and parentbird\_cond. 
 5. cond\_wait –  This works similarly to the mutex\_lock function by using the spinning process. This also keeps checking if the test-set condition for the conditional variable is true.
 6. cond\_signal – This helps signal the conditional variable to keep waiting again until a mutex variable calls it. 
 7. test\_set – This program implements the mutex variables and the conditional variables with the test and set lock mechanism. This is the system call that helps keep the process in the spinning state. The file name is test\_set. The function tests the mutex variable whose value it generally returns once the process has been temporarily allowed to run.  The lock is obtained when the it returns FALSE. The process is held in the while loop till any other process signals it or lets go of the lock of the mutex variable.

The mutex variables and the conditional variables were used to help enable or disable the interrupts. This lock mechanism tests the lock variable sent to the function and goes on to disable it. In the program, when the condition variable is signaled, the previous lock value is returned and enabled. This is to prevent the baby bird processes from running when the mother bird has gone to fetch the worms and vice versa. If the test and set function return True, the processes keep running till another process gets the signal or unlocks the mutex as required. 

Hence, the test-set function disable and enable by interrupts by:

 1. Initially the interrupts are disabled. (This was done using the mask example given in the question)
 2. Variable is initially tested and if it is NULL, returns a System error.
 3. Sets a temporary variable to the value of the variable sent to the test-set function. 
 4. Initializes the variable to 1, hence the interrupts can be enabled. 
 5.  Returns the value of the temporary variable that held the previous value of the mutex.

## Questions

```java
class AOS_Test1 implements Lock {
  private int turn;
  private boolean busy = false;
  public void lock() {
    int me = ThreadID.get();
    do {
      do {
        turn = me;
      } while (busy);
        busy = true;
    } while (turn != me);
  }
  public void unlock() {
    busy = false;
  }
}
```

 1. What is starvation? Is this protocol starvation-free?

Starvation occurs when a process is continuously denied access to a resource. A protocol is starvation-free whenever it is written in a way that guarantees each process will be bypassed only a finite number of times before it is eventually granted access to the resource it's waiting on. Source: https://en.wikipedia.org/wiki/Starvation_(computer_science)

This protocol is not starvation-free because it introduces a semi-race condition which prevents us from making any guarantees about which processes will be granted or denied access next. We are continuously updating the turn variable while busy-waiting so if we have multiple processes busy-waiting then they are all fighting over whose turn is next. Therefore there's nothing preventing one process from being continuously denied access to the resource, especially if one process is lower priority than the others.

 2. What is deadlock? Is this protocol deadlock-free?

Deadlock occurs when a number of processes have some resource(s) locked and are waiting on others to unlock in a circular manner that prevents any single process from continuing. Simple example: P1 has R1 locked and is waiting on R2 while P2 has R2 locked and is waiting on R1. A protocol is deadlock-free when it is written in such a way that a deadlock situation cannot occur. Source: https://en.wikipedia.org/wiki/Deadlock

This protocol is not deadlock-free because there is a chance we will set busy equal to true and then busy-wait until this busy variable becomes false. This sequence of events will result in deadlock assuming P1 and P2 are initially waiting on the lock which P3 will unlock.

 - P1 sets `turn = me` and is inturupted before checking the while-conditional
 - P2 sets `turn = me` and busy-waits for a bit.
 - P3 calls unlock()
 - P1 resumes, checks the while-conditional and exits the loop without setting `turn = me`
 - P1 sets `busy = true` and then restarts the outer loop because `turn != me`
 - P1 resumes busy-waiting because it just set `busy = true`
 - P2 continues to busy-wait, no progress can be made.

 
----------Work Contributed.
Bo Henderson worked on making the program more robust and drafted the questions in the report. All the exceptions handling were programmed by him including the files in the apps folder.


Burde Prerana Kamath worked on the system files and the files in the include folder along with files in the shell folder and drafted the working of the program. 


The working of the code was initially understood.

The skeleton of the code was discussed and the work was split equally. 

Debugging and testing were done together.

The answers to the questions were discussed and drafted. 