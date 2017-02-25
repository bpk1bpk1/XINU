#include <xinu.h>
syscall test_set(mutex_t* lock)
{
	if(lock == NULL)
	{
		return SYSERR;
	}

	intmask mask;
	mask = disable();
	mutex_t temp;
	temp = *lock; //This helps save the previous value of the lock
	*lock = 1; //This is initialised to 1 to not let any other mutex from running since this process is running
	restore(mask);
	

 return temp; //Once the process has finished doing the job, it will return the previous value of lock where the mutex will hold the prcess again
}


