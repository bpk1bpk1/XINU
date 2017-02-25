#include <xinu.h>
//This is when the process has finisht executing and to release the mutex
//holding the process from running.
syscall mutex_unlock(volatile mutex_t* lock)
{
	if(lock == NULL)
	{
		printf("Null pointer value");
		return SYSERR;
	}

	*lock = 0;

		return OK;

}
