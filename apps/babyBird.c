#include <babybird.h>
void babyBird(int id_bird)
{
  int i = 0;
  while(i < num_eat_worms)
    {
      mutex_lock(&babybird);
      if(wormsLeft == 0)
	{
	  cond_signal(&parentbird_cond);
	  mutex_lock(&babybird1);
	  cond_wait(&babybird_cond,&babybird1);
	  mutex_unlock(&babybird1);
	}

      kprintf(" \n Baby bird %d ate a worm!(%d total)", id_bird, i);
      wormsLeft--;
      i++;

      mutex_unlock(&babybird);
      resched();
    }

  babyEnds = babyEnds + 1;
  if(babyEnds == num_baby_birds)
    {
      cond_signal(&parentbird_cond);
    }
}

