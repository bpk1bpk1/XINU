#include <baby_bird.h>
void parentBird()
{
 	int i = 1;
	while(i == 1)
	{
		mutex_lock(&parentbird);
		cond_wait(&parentbird_cond, &parentbird);
		if(babyEnds == num_babybirds)
		{
			break;
		}
		
		printf(" \n The parent bird has fetched %d number of worms", num_getworms);
		wormsLeft = num_getworms;
		cond_signal(&babybird_cond);
		mutex_unlock(&parentbird);
    }

	printf(" \n Subsequent number of worms is: %d", wormsLeft);
}

		
