/* xsh_process_ring.c - xsh_process_ring */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process_ring.h>

// function prototypes
void processring(int);
void process_arg_pair(char* key, char* val);

volatile int count = 0;

// maximum accepted values for number of process/rounds
int MAXPROC = 30;
int MAXROUND = 2147483647;

enum modes { WORK, HANG, LOOP, CHAOS};
static int rounds;
static int processes;
static int mode;

// help text describing how the program should be used
// printed when the user calls this program incorrectly
char* usage = "\nUsage: process_ring [OPTIONS]\n\nOPTIONS\n    -p, --processes NUM\n        sets the number of processes to NUM. NUM must be an integer between 0 and 60 (default: 4)\n    -r, --rounds NUM\n        sets the number of rounds to NUM. NUM must be an integer between 0 and 60 (default: 5)\n    -v, --version MODE\n        where MODE is one of:\n            - work (works correctly, default behavior)\n            - hang (does nothing)\n            - loop (goes into an infinite loop)\n            - chaos (prints numbers out of order)\n\n";

/* -----------------------------------------------------------------------
 * xsh_process_ring - print a countdown using several processes
 * -----------------------------------------------------------------------
 */
shellcmd xsh_process_ring(int argc, char *argv[]) 
{
  
  // set defaults for our mode, number of processes, and number of rounds
  processes = 4;
  rounds = 5;
  mode = WORK;

  // print help message
  if (strncmp(argv[1], "--help", 7) == 0 || strncmp(argv[1], "-h", 3) == 0)
    {
      printf("%s", usage);
      return 0;
    }
  
  // we should never be processing more than 6 args plus arg[0]
  if (argc > 7) 
    {
      printf("\nError: too many arguments\n%s", usage);
      return 1;
    }
  
  // every --specifier should be followed by exactly one other arguments 
  // so we should always have an even number of arguments..
  // including arg[0], that means argc should always be odd
  if (argc == 2 || argc == 4 || argc == 6) 
    {
      printf("\nError: unpaired arguments\n%s", usage);
      return 1;
    }
  
  // process 1st pair of command line arguments if given
  if (argc >= 3) { process_arg_pair(argv[1], argv[2]); }
  // process 2nd pair command line arguments if given
  if (argc >= 5) { process_arg_pair(argv[3], argv[4]); }
  // process 3rd pair of command line arguments if given
  if (argc == 7) { process_arg_pair(argv[5], argv[6]); }
  
  printf("Number of Processes: %d\nNumber of Rounds: %d\nMode: %d\n\n", processes, rounds, mode);

  int i;
  printmessage[0] = count = numberofprocesses * rounds;
  for (i = 0; i < numberofprocesses; i++)
    {
      resume (create (processring, 1024, 20, "i", 1, i));
    }

  return 0;

}


void process_arg_pair(char* key, char* val) 
{
  if (strncmp(key, "-p", 3) == 0 || strncmp(key, "--processes", 12) == 0 || strncmp(key, "--process", 10) == 0) 
    {
      processes = atoi(val);
      if (processes < 1 || processes > MAXPROC)
	{
	  printf("\nError: invalid number of processes\n%s", usage);
	  exit(1);
	}
    } 
  else if (strncmp(key, "-r", 3) == 0 || strncmp(key, "--rounds", 9) == 0 || strncmp(key, "--round", 8) == 0) 
    {
      rounds = atoi(val);
      if (rounds < 1 || rounds > MAXROUND)
	{
	  printf("\nError: invalid number of rounds\n%s", usage);
	  exit(1);
	}
    } 
  else if (strncmp(key, "-v", 3) == 0 || strncmp(key, "--version", 10) == 0) 
    {
      if (strncmp(val, "work", 5) == 0)
	{
	  mode = WORK;
	}
      else if (strncmp(val, "hang", 5) == 0)
	{
	  mode = HANG;
	}
      else if (strncmp(val, "loop", 5) == 0)
	{
	  mode = LOOP;
	}
      else if (strncmp(val, "chaos", 6) == 0)
	{
	  mode = CHAOS;
	}
      else
	{
	  printf("\nError: invalid version\n%s", usage);
	  exit(1);
	}
    }
  else
    {
      printf("\nError: invalid argument\n%s", usage);
      exit(1);
    }
}
