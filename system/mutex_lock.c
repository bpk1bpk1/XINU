#include <xinu.h>
syscall mutex_lock(volatile mutex_t* lock)
{

	if(lock == NULL)
	{
		printf(" \n Null Pointer Value");
		return SYSERR;
	}

	while(previous)
	{
		intmask mask;
		mask = disable();
		mutex_t = previous;
		previous = *lock;
		*lock = 1;
		restore(mask);
	}

	return OK;
}
