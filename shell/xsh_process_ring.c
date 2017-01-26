/* xsh_process_ring.c - xsh_process_ring */

#include <xinu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process_ring.h>

/* -----------------------------------------------------------------------
 * xsh_process_ring - print a countdown using several processes
 * -----------------------------------------------------------------------
 */
shellcmd xsh_process_ring(int argc, char *argv[]) 
{
  // set some defaults
  processes = 4;
  rounds = 5;
  version = WORK;

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

  printf("Number of Processes: %d\nNumber of Rounds: %d\n", processes, rounds);

  // initialize our volatile variables
  count = processes * rounds;

  // create our semaphores
  sid32 proctrl = semcreate(0);
  sid32 isdone = semcreate(0);

  int i;
  for (i = 0; i < processes; i++) {
    if (version == CHAOS) {
      resume(create(chaosring, 1024, 20, "process_ring", 2, i, isdone));
    } else {
      resume(create(processring, 1024, 20, "process_ring", 3, i, proctrl, isdone));
    }
  }

  // get things started if we aren't hanging
  if (version != HANG) { signal(proctrl); }

  // wait for above processes to finish so the prompt doesn't print too early
  wait(isdone);
  sleep(0.5);
  return 0;
}


void printErr(char* msg) {
  // help text describing how the program should be used
  // printed when the user calls this program incorrectly
  char* usage = "\nUsage: process_ring [OPTIONS]\n\nOPTIONS\n    -p, --processes NUM\n        sets the number of processes to NUM. NUM must be an integer between 0 and 60 (default: 4)\n    -r, --rounds NUM\n        sets the number of rounds to NUM. NUM must be an integer between 0 and 60 (default: 5)\n    -v, --version STRING\n        where STRING is one of:\n            - 'work' (works correctly, default behavior)\n            - 'hang' (does nothing)\n            - 'loop' (goes into an infinite loop)\n            - 'chaos' (prints numbers out of order)\n    -h, --help\n        print this help message\n\n";
  printf("\n%s\n%s", msg, usage);
  exit(1);
}


void parse_argv_pair(char* key, char* val) 
{
  // try to extract the number of processes
  if (strncmp(key, "-p", 3) == 0 || strncmp(key, "--processes", 12) == 0 || strncmp(key, "--process", 10) == 0) 
    {
      processes = atoi(val);
      if (processes < 1 || processes > MAXPROC) { printErr("Error: invalid number of processes"); }
    } 

  // try to extract the number of rounds
  else if (strncmp(key, "-r", 3) == 0 || strncmp(key, "--rounds", 9) == 0 || strncmp(key, "--round", 8) == 0) 
    {
      rounds = atoi(val);
      if (rounds < 1 || rounds > MAXROUND) { printErr("Error: invalid number of rounds"); }
    } 

  // try to extract the version
  else if (strncmp(key, "-v", 3) == 0 || strncmp(key, "--version", 10) == 0) 
    {
      if (strncmp(val, "work", 5) == 0)	{ version = WORK; }
      else if (strncmp(val, "hang", 5) == 0) { version = HANG; }
      else if (strncmp(val, "loop", 5) == 0) { version = LOOP; }
      else if (strncmp(val, "chaos", 6) == 0) { version = CHAOS; }
      else { printErr("Error: invalid version"); }
    }
  else { printErr("Error: invalid argument"); }
}
