#include <xinu.h>
syscall mutex_unlock(volatile mutex_t* lock)
{
	if(lock == NULL)
	{
		printf("Null pointer value");
		return SYSERR;
	}

	*lock = 0 //Reinitialising the value 

		return OK;

}
