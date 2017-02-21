#include <babybird.h>
#include <string.h>
#include <stdio.h>

#define INT_MAX 2000000000
#define MAXBIRDS 60

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

	// print help message
	if (strncmp(argv[1], "--help", 7) == 0 || strncmp(argv[1], "-h", 3) == 0)  { printErr("Help"); }
  
	// we should never be processing more than 6 args plus arg[0]
	if (argc > 7) { printErr("Error: too many arguments"); }
  
	// every --specifier should be followed by exactly one other arguments 
	// including arg[0], that means argc should always be odd
	if (argc == 2 || argc == 4 || argc == 6) { printErr("Error: unpaired arguments"); }
  
	// process 1st pair of command line arguments if given
	if (argc >= 3) { parse_argv_pair(argv[1], argv[2]); }
	// process 2nd pair command line arguments if given
	if (argc >= 5) { parse_argv_pair(argv[3], argv[4]); }
	// process 3rd pair of command line arguments if given
	if (argc == 7) { parse_argv_pair(argv[5], argv[6]); }

	printf("Number of Birds: %d \tFetch Number: %d \tWorms to eat: %d\n", num_babybirds, num_getworms, num_eatworms);

	// exit(0);

	resume(create(parentBird, 1024,20,"Parent bird",0));

	for(i = 0; i< num_babybirds ; i++)
	{
		resume(create(babyBird, 1024, 20, "Baby bird",1,i));

	}
	
	return 0;
}



void printErr(char* msg) {
  // help text describing how the program should be used
  // printed when the user calls this program incorrectly
  char* usage = "\nUsage: baby_bird [OPTIONS]\n\nOPTIONS\n    -b, --babies NUM\n        sets the number of baby birds to NUM. NUM must be an integer\n    -f, --fetch NUM\n        sets the number of worms the mother fetches at a time. NUM must be an integer\n    -e, --eat NUM\n        sets the number of worms each baby bird needs to eat. NUM must be an integer.\n    -h, --help\n        print this help message\n\n";
  printf("\n%s\n%s", msg, usage);
  exit(1);
}


void parse_argv_pair(char* key, char* val) 
{
  // try to extract the number of processes
  if (strncmp(key, "-b", 3) == 0 || strncmp(key, "--babies", 8) == 0) 
    {
      num_babybirds = atoi(val);
      if (num_babybirds < 1 || num_babybirds > MAXBIRDS) { printErr("Error: invalid number of baby birds"); }
    } 

  // try to extract the number of rounds
  else if (strncmp(key, "-f", 3) == 0 || strncmp(key, "--fetch", 8) == 0) 
    {
      num_getworms = atoi(val);
      if (num_getworms < 1 || num_getworms > INT_MAX) { printErr("Error: invalid number of worms to fetch"); }
    } 

  // try to extract the version
  else if (strncmp(key, "-e", 3) == 0 || strncmp(key, "--eat", 6) == 0) 
    {
      num_eatworms = atoi(val);
      if (num_eatworms < 1 || num_eatworms > INT_MAX) { printErr("Error: invalid number of worms to eat"); }
    } 
  else { printErr("Error: invalid argument"); }
}




