#include <babybird.h>
#include <stdio.h>
#include <string.h>


void parentBird()
{
  int i = 1;
  while(i == 1)
    {
      mutex_lock(&parentbird); //This is the mutex for the mother
      cond_wait(&parentbird_cond, &parentbird); //Spinning process
	  //printf("The number of babyends %d \n", babyEnds); Debugging
      if(babyEnds == num_baby_birds)
	{
	  break; //For the parent process to end once the babies have ended.
	}
	  	
      kprintf(" \n The parent bird has fetched %d number of worms", num_fetch_worms);
      wormsLeft = num_fetch_worms;  //This is when the bird fetches the worms once the baby bird has signalled it.
	  kprintf(" \n Parent bird filled the dish with %d worms", num_fetch_worms);
      cond_signal(&babybird_cond); //This signals the babybird to start eating again
      mutex_unlock(&parentbird); 
    }
 //printf("Number of worms left :%d", wormsLeft); 
  kprintf(" \n The  number of worms is: %d. Hence, none of the baby birds have starved and they all live Happily ever After! :)", wormsLeft); //This is for the parent process to print the number of worms left
}
