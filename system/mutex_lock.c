#include <xinu.h>
//This is for the process to keep running and waiting for the coniditon to be
//satisfied. This will keep the while loop running - hence the process keeps
//running ill the lock mechanism doesnt return False
syscall mutex_lock(volatile mutex_t* lock)
{

	if(lock == NULL)
	{
		printf(" \n Null Pointer Value");
		return SYSERR;
	}

	while(test_set(lock));

	return OK;
}
