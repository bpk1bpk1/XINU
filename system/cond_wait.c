#include <xinu.h>
//This is when the process needs to wait for it to be called.
//The process has the mutex variable lock
//This will initially release the variable lock
//Finish the work it was meant to do
//Lock the mutex for the process not to run again
syscall cond_wait(volatile cond_t* cv, volatile mutex_t* lock)
{
	if(lock == NULL || cv == NULL)
	{
		printf("Null pointer values");
		return SYSERR;
	}

	mutex_unlock(lock);
	while(test_set(cv));

	mutex_lock(lock); //Reinitialising the whole thing
	return OK;
}
