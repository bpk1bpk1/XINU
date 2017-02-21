#include <babybird.h>
#include <string.h>
#include <stdio.h>

volatile int babyEnds = 0;
volatile int wormsLeft = 0;
int num_getworms = 0;
int num_eatworms = 0;
int num_babybirds = 0;
volatile cond_t babybird_cond;
volatile cond_t parentbird_cond;
volatile mutex_t babybird;
volatile mutex_t parentbird;
volatile mutex_t babybird1;

shellcmd xsh_babybird(int32 argc, char *argv[]) {
	int i;
	mutex_create(&babybird);
	mutex_create(&parentbird);
	mutex_create(&babybird1);
	cond_init(&babybird_cond);
	cond_init(&parentbird_cond);
	//default values
	num_babybirds = 5;
	num_getworms = 2;
	num_eatworms = 3;
	babyEnds = 0;
	wormsLeft = 0;

	if(strcmp(argv, "--help", 7) == 0 || strcmp(argv, "-h", 3) == 0)
	{
		printf("Help");
		printf(" \n Usage : <-b> <Number of baby birds> <-f> <Number of fetch worms> <-e> <Number of worms to eat>"); 
	}

	if(argc > 1 && argc < 6)
	{
		printf(" \n Too few arguments");

	}

	if(argc >= 7)
	{
		printf("Too many arguments");
	}

	num_babybirds = atoi(argv[2]);
	num_getworms = atoi(argv[4]);
	num_eatworms = atoi(argv[6]);

	resume(create(parentBird, 1024,20,"Parent bird",0));

	for(i = 0; i< num_babybirds ; i++)
	{
		resume(create(babyBird, 1024, 20, "Baby bird",1,i));

	}
	
	return 0;
}









