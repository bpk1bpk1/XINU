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
	temp = *lock;
	*lock = 1;
	restore(mask);
	

 return temp;
}


