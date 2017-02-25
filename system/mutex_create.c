#include <xinu.h>
//This helps create the mutex variable
syscall mutex_create(volatile mutex_t* lock)
{
	if(lock == NULL)
	{
		printf(" \n Null Pointer Value");
		return SYSERR;
	}

		*lock = 0;
		return OK;	

}


