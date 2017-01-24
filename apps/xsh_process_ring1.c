#include <xinu.h>
#include <kernel.h>
#include <process_ring.h>

volatile int printmessage[numberofprocesses];

void ring_process (int ring_element)
{
  int round = 0;
  while (1)
    {

      //Runs in order
      if (count % numberofprocesses ==
	  (numberofprocesses - ring_element) % numberofprocesses)
	{
	  printf ("Ring Element %d : Round %d : Value %d \n ", ring_element,
		   round++,printmessage [ring_element]);
	  printmessage[(ring_element + 1) % numberofprocesses] = --count;

	  if (count == 0)
	    {
	      count--;
	      printf ("Zero!");
	      return;
	    }
	}
    }
}
