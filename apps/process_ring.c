
#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process_ring.h>

// pi = process index
void processring (int pi, sid32 proctrl, sid32 isdone)
{
  while (1)
    {
      wait(proctrl);
      // exit and signal if we're finished
      if (count < 0 && version != LOOP) 
	{
	  signal(isdone);
	  exit(0); 
	}
      // otherwise print and decrement the count
      else 
	{
	  if (count == 0) { 
	    printf("ZERO!\n"); 
	  } else {
	    printf("Ring Element %d : Round %d : Value %d\n", pi, rounds-1-((count-1)/processes), count); 
	  }
	  count--;
	}
      // next process in line can do their thing
      signal(proctrl);
    }
}


void chaosring (int pi, sid32 isdone)
{
  int private_count = (processes * rounds) - pi;
  intmask mask; // for re-enabling interrupts
  while (1)
    {

      // I don't want to be interrupted in the middle of an iteration
      mask = disable();

      // exit and signal if we're finished ()
      if ((private_count < 0 || count < 0) && version != LOOP) 
	{
	  sleep(0.2);
	  signal(isdone);
	  exit(0); 
	}
      // otherwise print and decrement our private count
      else 
	{
	  if (private_count == 0) { 
	    printf("ZERO!\n"); 
	  } else if (version == CHAOS) { 
	    printf("Ring Element %d : Round %d : Value %d\n", pi, rounds-1-((private_count-1)/processes), private_count); 
	  }
	  private_count -= processes;
	}

      // re-enable interrupts
      restore(mask); 
      // sleep so that other processes can catch up to make things more chaotic >:)
      sleep(0.1);
      
    }
}
