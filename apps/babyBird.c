#include <babybird.h>
void babyBird(int id_bird)
{
  int i = 0;
  while(i < num_eat_worms)  //The total number of worms left for the birds to eat
    {
      mutex_lock(&babybird); //To get the initial mutex of babybird
      if(wormsLeft == 0) //This is the signal to the conditional variable of the parent to go and fetch worms
	{
	  cond_signal(&parentbird_cond);
	  mutex_lock(&babybird1); //This helps keep the babybirds from eating the worms while the parent is fetching the worms
	  cond_wait(&babybird_cond,&babybird1);
	  mutex_unlock(&babybird1);
	}

      kprintf(" \n Baby bird %d ate a worm!(%d total)", id_bird, i); //The print statement
      wormsLeft--; //Reduces the present number of worms for the final print statement that the parent prints to show the worms left since it is a global variable
      i++; //Increases the number of worms to match it to the number of fetched worms.

      mutex_unlock(&babybird); //Unlocking the mutex of the babybird when all the babies have finisht eating
      resched();
    }

  babyEnds = babyEnds + 1; //This tells us the number of baby birds that have completely finished eating
  if(babyEnds == num_baby_birds)
    {
      cond_signal(&parentbird_cond); //This is a signal for the parent process to end by showing the number of worms left
    }
}

