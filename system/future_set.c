#include <xinu.h>
#include <future.h>

syscall
future_set (future_t * f, int value)
{
  if (f->state == FUTURE_READY)
    {
      if (f->mode == FUTURE_EXCLUSIVE || f->mode == FUTURE_SHARED)
	{
	  printf
	    ("The mode is exclusive or shared , Please change the mode\n");
	  return SYSERR;
	}
      else
	{
	  pid32 pid = getpid ();

	  enqueue (pid, f->set_queue);
	  if (suspend (pid) == SYSERR)
	    {
	      printf ("Suspend failed");
	    }
	}
    }
  if (f->state == FUTURE_EMPTY || f->state == FUTURE_WAITING)
    {
      if (isempty (f->get_queue))
	{
	  pid32 pid = getpid ();

	  enqueue (pid, f->set_queue);
	  suspend (pid);
	}
      f->state = FUTURE_READY;
      f->value = value;
      pid32 pid;
      if (f->mode == FUTURE_QUEUE)
	{
	  if ((pid = dequeue (f->get_queue)) != EMPTY)
	    {

	      if (resume (pid) == SYSERR)
		{
		  printf ("Resume failed");
		}
	    }
	}
      else
	{
	  while ((pid = dequeue (f->get_queue)) != EMPTY)
	    {
	      if (resume (pid) == SYSERR)
		{
		  printf ("Resume failed");
		}
	    }
	}

    }
  return OK;
}
