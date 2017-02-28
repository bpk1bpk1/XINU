#include <xinu.h>
#include <future.h>

syscall future_get (future_t * f, int *value)
{
  if (f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING) //Checking for the pointer conditions
    {
      if (f->mode == FUTURE_EXCLUSIVE && !isempty (f->get_queue))
	{
	  printf ("The mode is exclusive , Please change the mode\n");
	  return SYSERR;
	}
      pid32 pid = getpid ();
      f->state = FUTURE_WAITING;
      pid = enqueue (pid, f->get_queue);
      if (suspend (pid) == SYSERR)
	{
	  printf ("Suspend failed");
	}
    }

  if (f->state == FUTURE_READY)
    {
      pid32 pid;
      if (f->mode == FUTURE_QUEUE)
	{
	  if ((pid = dequeue (f->set_queue)) != EMPTY)
	    {
	      f->state = FUTURE_WAITING;
	      if (resume (pid) == SYSERR)
		{
		  printf ("Resume failed");
		}
	    }
	  else
	    {
	      f->state = FUTURE_WAITING;
	    }
	}
      if (isempty (f->get_queue))
	f->state = FUTURE_EMPTY;
      *value = f->value;
    }
  return OK;
}
