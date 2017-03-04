#include <xinu.h>
//This program is to initialise the conditional variables
syscall cond_init(volatile cond_t* cv)
{
	if(cv == NULL)
	{
		printf("Null pointer value");
		return SYSERR;
	}


	*cv = 1;
	return OK;
}