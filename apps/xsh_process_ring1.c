#include <xinu.h>
#include <kernel.h>
#include <process_ring.h>

volatile int variable1[numberofprocesses];

void processring (int ring_element)
{
  int round = 0;
  while (1)
    {

      //Runs in order
      if (variable2 % numberofprocesses == (numberofprocesses - ring_element) % numberofprocesses)
	{
	  printf ("Ring Element %d : Round %d : Value %d \n ", ring_element,round++,variable1[ring_element]);
	  variable1[(ring_element + 1) % numberofprocesses] = --variable2;

	  if (variable2 == 0)
	    {
	      variable2--;
	      printf ("Zero!!");
	      return;
	    }
	}
    }
}
