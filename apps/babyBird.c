#include <babybird.h>
void babyBird(int id_bird)
{
	int i = 0;
	while(i < num_eatworms)
	{
		mutex_lock(&babybird);
		if(wormsLeft == 0)
		{
			cond_signal(&parentbird_cond);
			mutex_lock(&babybird1);
			cond_wait(&babybird_cond,&babybird1);
			mutex_unlock(&babybird1);
		}

	printf(" \n Baby bird %d ate a worm!(%d total)", id_bird, wormsLeft);
	wormsLeft = wormsLeft - 1;
	i++;

	mutex_unlock(&babybird);
	resched();
}

babyEnds = babyEnds + 1;
if(babyEnds == num_babybirds)
{
	cond_signal(&parentbird_cond);
}
}

