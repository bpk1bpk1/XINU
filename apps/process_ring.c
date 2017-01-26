#include <xinu.h>
#include <kernel.h>
#include <process_ring.h>

// pi = process index
void processring (int pi, sid32 ctrler)
{
  round = 0;

  while (1)
    {

      wait(ctrler);

      if (count < 0) { 
	exit(0); 
      }
      else if (count == 0) { 
	printf("ZERO!\n\n"); count--; exit(0);
      }
      else 
	{
	  printf ("Ring Element %d : Round %d : Value %d \n ", pi, round, count--);
	  if (count % processes == 0) { round++; }
	  signal(ctrler);
	}
      
      continue;

      if (count % rounds == 0) { round++; }


      // the process
      if (rounds - (count % rounds) - 1 == pi) {
      
	// don't continue past zero unless we're infinite-looping
	if (count < 0 && version != LOOP) { exit(0); }
	if (count == 0)
	  {
	    printf ("ZERO!\n");
	    count--;
	  }
	else 
	  {
	    printf ("Ring Element %d : Round %d : Value %d \n ", pi, round, count--);
	  }

	// and increment the round if the count is divisible by the number of rounds
	if (count % rounds == 0) { round++; }

      }



      /*
      //Runs out of order
      if(variable2 % numberofprocesses == (numberofprocesses - ring_element) % numberofprocesses)
	{
	  variable1[(ring_element + 1) % numberofprocesses] = --variable2;
	  printf("Ring Element %d : Round %d : Value %d \n ",ring_element,round++,variable1[ring_element]);	
		
	  if(variable2 == 0)
	    {
	      variable2--;
	      printf ("Zero!!\n\n");
	      return;
	    }
	}
      */

	
      /*
      //Out of order second try
      if(variable2%numberofprocesses == (numberofprocesses-ring_element)%numberofprocesses)
	{
	  variable1[(ring_element+1)%numberofprocesses]=--variable2;
	  printf("Ring Element %d : Round %d : Value %d \n", ring_element,round++,variable1[ring_element]);
		
	  if(variable2 == 0)
	    {
	      variable2--;
	      printf ("Zero!!\n\n");
	      return;
	    }
	}
      */
      

      /*
      //Enters a deadlock
      if(variable1[ring_element]%numberofprocesses == (numberofprocesses - ring_element) % numberofprocesses)
	{
	  variable1[ring_element] = variable2;
	  variable1[(ring_element + 1) % numberofprocesses ] = --variable2;
	  printf("Ring Element %d : Round %d : Value %d \n", ring_element, round++ , variable1[ring_element]);
	  --variable2;

	  if(variable2 == 0)
	    {
	      variable2--;
	      printf ("Zero!!\n\n");
	      return;
	    }
	}
      */

      
      /*
      //Enters an infinite loop
      if(variable1[ring_element] % numberofprocesses == (numberofprocesses - ring_element)%numberofprocesses)
	{
	  variable2 = variable1[ring_element];
	  printf("Ring Element: %d : Round : %d : Value : %d \n", ring_element,round++,variable1[ring_element]);
	  variable1[(ring_element + 1) % numberofprocesses] = --variable2;

	  if(variable2 == 0)
	    {
	      variable2--;
	      printf ("Zero!!\n\n");
	      return;
	    }
	}
      if(variable2 < 0)
	{
	  return;
	}
      */

    }
}
