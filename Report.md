# Assignment 6: Baby Bird Problem

### rojahend & bkamath

Initially, the mother bird process is created and it obtains the parent bird mutex variable lock and is waiting on the conditional variable. The baby bird’s processes are also created. The program has two conditional variables – one for the parent and one for the baby birds. The mutex and the conditional variable used in the system calls header were defined in the kernel file. 

Since the number of worms the mother bird has fetched is initially zero, the baby bird is going to acquire the baby bird mutex and signal the conditional variable of the parents. The baby bird is going to wait on its conditional variable and the temporary baby bird mutex till the mother can go and fetch the worms. The mother bird process is going to fill the maximum number of worms, signal the baby bird conditional variable and wait on its conditional variable. The baby bird is going to acquire the temporary baby bird mutex, since it has already acquired the baby bird mutex (waiting for the worms)when the parent signals its conditional variable. The baby bird then eats the worm and goes back in queue by unlock the baby bird mutex. This is the way the conditional variables are implemented. For the program to end, every time a baby bird process has eaten its share of worms and it exits, a variable babyEnds is incremented. Once the babyEnds is equivalent to the number of birds, then the bird who eats the last worm signals the mother’s conditional variable. The mother bird’s process checks the condition if all its babies have eaten and since the if condition will be true, the programs breaks from the loop. It prints the number of worms left since it’s a global variable that kept getting decremented. It then exits the program.

The following system files were created in the program (These headers were initialized in the prototypes file in the include folder): 

 1. mutex\_create – This helped created the mutex variables for the program. Babybird, parentbird and temporarybabybird
 2. mutex\_lock – In this system call, the mutex’s keep checking if the condition in the test-set function is true. This process of checking the conditional variable is called spinning.
 3. mutex\_unlock – This helps unlock the mutex variables.
 4. cond\_init – This helped initialize the conditional variables babybird\_cond and parentbird\_cond. 
 5. cond\_wait –  This works similarly to the mutex\_lock function by using the spinning process. This also keeps checking if the test-set condition for the conditional variable is true.
 6. cond\_signal – This helps signal the conditional variable. 
 7. test-set – This program implements the mutex variables and the conditional variables with the test and set lock mechanism. This is the system call that helps keep the process in the spinning state. The file name is test-set. The function tests the lock variable whose value it generally returns or sets the lock.  The lock is obtained when the it returns FALSE. If the function returns TRUE, some other process is already holding the lock.

The mutex variables and the conditional variables were used to help enable or disable the interrupts. This lock mechanism tests the lock variable sent to the function and goes on to disable it. In the program, when the condition variable is signaled, the previous lock value is returned and enabled. This is to prevent the baby bird processes from running when the mother bird has gone to fetch the worms and vice versa. If the test and set function return True, the processes keep running till another process gets the signal or unlocks the mutex as required. 

Hence, the test-set function disable and enable by interrupts by:

 1. Initially the interrupts are disabled.
 2. Variable is initially tested and if it is NULL, returns a System error.
 3. Sets a temporary variable to the value of the variable sent to the test-set function. 
 4. Initializes the variable to 1, hence enabling the interrupts too. 
 5.  Returns the value of the temporary variable. 

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
