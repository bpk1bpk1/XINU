#include <xinu.h>
syscall mutex_create(volatile *mutex_t lock)
{
	if(lock == NULL)
	{
		printf(" \n Null Pointer Value");
		return SYSERR;
	}

		*lock = 0;
		return OK;	

}


