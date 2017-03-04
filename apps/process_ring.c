#include <xinu.h>
#include <kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process_ring.h>

void
processring (int ring_element, sid32 * proctrl, sid32 isdone)
{
  if (version == WORK)
    {
      workingring (ring_element, proctrl, isdone);
    }
  else if (version == HANG)
    {
      hangring (ring_element);
    }
  else if (version == LOOP)
    {
      loopring (ring_element);
    }
  else if (version == CHAOS)
    {
      chaosring (ring_element, isdone);
    }
}


void
workingring (int ring_element, sid32 * proctrl, sid32 isdone)
{
  while (1)
    {
      wait (proctrl[ring_element]);


      if (count == 0)
	{
	  kprintf ("ZERO!\n");
	}
      else
	{
	  kprintf ("Ring Element %d : Round %d : Value %d\n", ring_element,
		   rounds - 1 - ((count - 1) / processes), count);
	}
      count--;


      if (count < 0)
	{
	  signal (isdone);
	  exit (0);
	}


      if (ring_element == processes - 1)
	{
	  signal (proctrl[0]);
	}
      else
	{
	  signal (proctrl[ring_element + 1]);
	}
    }
}


void
hangring (int ring_element)
{
  while (1)
    {
      if (variable1[ring_element] % processes ==
	  (processes - ring_element) % processes)
	{
	  variable1[ring_element] = count;
	  variable1[(ring_element + 1) % processes] = --count;
	  kprintf ("Ring Element %d : Round %d : Value %d\n", ring_element,
		   rounds - 1 - ((count - 1) / processes), count);
	  --count;

	  if (count == 0)
	    {
	      count--;
	      kprintf ("Zero!!");
	      return;
	    }
	}
    }
}


void
loopring (int ring_element)
{
  while (1)
    {
      if ((count + ring_element) % processes == 0)
	{
	  kprintf ("Ring Element %d : Round %d : Value %d\n", ring_element,
		   rounds - 1 - ((count - 1) / processes), count--);
	}
    }
  if (count <= 0)
    {
      kprintf ("Zero!!");
      return;
    }
}


void
chaosring (int ring_element, sid32 isdone)
{
  int private_count = (processes * rounds) - ring_element;
  while (1)
    {
      if (private_count == 0)
	{
	  kprintf ("ZERO!\n");
	}
      else if (version == CHAOS)
	{
	  kprintf ("Ring Element %d : Round %d : Value %d\n", ring_element,
		   rounds - 1 - ((private_count - 1) / processes),
		   private_count);
	}
      private_count -= processes;
      count--;


      if (count < 0)
	{
	  signal (isdone);
	}


      if (private_count < 0)
	{
	  exit (0);
	}


      sleep (0.1);
    }
}