#include <babybird.h>
void parentBird()
{
  int i = 1;
  while(i == 1)
    {
      mutex_lock(&parentbird);
      cond_wait(&parentbird_cond, &parentbird);
      if(babyEnds == num_baby_birds)
	{
	  break;
	}
		
      kprintf(" \n The parent bird has fetched %d number of worms", num_fetch_worms);
      wormsLeft = num_fetch_worms;
      cond_signal(&babybird_cond);
      mutex_unlock(&parentbird);
    }

  kprintf(" \n Subsequent number of worms is: %d", wormsLeft);
}
