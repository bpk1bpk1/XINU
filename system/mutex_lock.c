#include <xinu.h>
syscall mutex_lock(volatile mutex_t* lock)
{

	if(lock == NULL)
	{
		printf(" \n Null Pointer Value");
		return SYSERR;
	}

	while(test-set(lock));

	return OK;
}
