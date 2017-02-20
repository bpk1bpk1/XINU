#include <xinu.h>
syscall cond_signal(volatile cond_t* cv)
{
	if(cv == NULL)
	{
		printf("\n Null pointer value");
		return SYSERR;
	}
	 
	*cv = 0;

	return OK;
}
