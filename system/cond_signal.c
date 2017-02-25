#include <xinu.h>
//This is when the process has finisht its work or the work it was waiting for
//has been done and it can resume its work.
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
