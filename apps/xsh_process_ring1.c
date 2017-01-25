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
     
	/*if (variable2 % numberofprocesses == (numberofprocesses - ring_element) % numberofprocesses)
	{
	  printf ("Ring Element %d : Round %d : Value %d \n ", ring_element,round++,variable1[ring_element]);
	  variable1[(ring_element + 1) % numberofprocesses] = --variable2;

	  if (variable2 == 0)
	    {
	      variable2--;
	      printf ("Zero!!");
	      return;
	    }
	}*/


	   //Runs out of order
	/*if(variable2 % numberofprocesses == (numberofprocesses - ring_element) % numberofprocesses)
	{
		variable1[(ring_element + 1) % numberofprocesses] = --variable2;
		printf("Ring Element %d : Round %d : Value %d \n ",ring_element,round++,variable1[ring_element]);	
		
		if(variable2 == 0)
		{
			variable2--;
			printf("Zero!!");
			return;
			}
	}*/

		//Out of order second try
		if(variable2%numberofprocesses == (numberofprocesses-ring_element)%numberofprocesses)
		{
			variable1[(ring_element+1)%numberofprocesses]=--variable2;
			printf("Ring Element %d : Round %d : Value %d \n", ring_element,round++,variable1[ring_element]);
		
			if(variable2 == 0)
			{
				variable2--;
				printf("Zerooo!!");
				return;
				}
		}


		//Enters a deadlock
		/*if(variable1[ring_element]%numberofprocesses == (numberofprocesses - ring_element) % numberofprocesses)
		{
			variable1[ring_element] = variable2;
			variable1[(ring_element + 1) % numberofprocesses ] = --variable2;
			printf("Ring Element %d : Round %d : Value %d \n", ring_element, round++ , variable1[ring_element]);
			--variable2;

			if(variable2 == 0)
			{
				variable2--;
				printf("Zero!!");
				return;
			}
		}*/


		//Enters an infinite loop
	 /*if(variable1[ring_element] % numberofprocesses == (numberofprocesses - ring_element)%numberofprocesses)
		{
			variable2 = variable1[ring_element];
			printf("Ring Element: %d : Round : %d : Value : %d \n", ring_element,round++,variable1[ring_element]);
			variable1[(ring_element + 1) % numberofprocesses] = --variable2;

			if(variable2 == 0)
			{
				variable2--;
				printf("Zero!!");
				return;
				}
		}*/

	if(variable2 < 0)
	{
		return;
	}


    }
}
