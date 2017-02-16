/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int nphilos;

#define MAXPHILOS 10
#define MAXCYCLES 10
#define MAXMAXDELAY 5000
#define MINALL 1
#define REQNARGS 4

#define LEFT (i + nphilos - 1) % nphilos
#define RIGHT (i + 1) % nphilos

void every_philosopher (int i);
void pickup (int i);
void putit_back (int i);

void thinking (int i);
void testing (int i);
void eating (int i);

sid32 mutex;
sid32 sem[10];
int maxdelay;
int state[10];



/*------------------------------------------------------------------------
 *  * xsh_date - obtain and print the current month, day, year, and time
 *   *------------------------------------------------------------------------
 *    */
shellcmd
xsh_dining_philosophers (int nargs, char *args[])
{

  int ncycles;			// in bytes
  int temp = 1; 
  if (nargs == 1)
    {
      nphilos = 10;
      ncycles = 10;
      maxdelay = 1000;
    }
  else if (nargs > REQNARGS)
    {
      kprintf ("Error, too many arguments. Expected %d.\n", REQNARGS - 1);
      return 1;
    }
  else if (nargs < REQNARGS)
    {
      kprintf ("Error, too few arguments. Expected %d.\n", REQNARGS - 1);
      return 1;
    }
  else
    {
      nphilos = atoi (args[1]);
      if (nphilos > MAXPHILOS || nphilos < MINALL)
	{
	  kprintf ("Error, invalid number of philosophers: %d\n", nphilos);
	  return 1;
	}

      ncycles = atoi (args[2]);
      if (ncycles > MAXCYCLES || ncycles < MINALL)
	{
	  kprintf ("Error, invalid number of cycles: %d\n", ncycles);
	  return 1;
	}


      maxdelay = atoi (args[3]);
      if (maxdelay > MAXMAXDELAY || maxdelay < MINALL)
	{
	  kprintf ("Error, invalid max delay: %d\n.", maxdelay);
	  return 1;
	}
    }

  kprintf ("Philosophers: %d\tCycles: %d\tMax Delay: %d\n", nphilos, ncycles,
	   maxdelay);
  mutex = semcreate (1);
  int j;
  for (j = 0; j < nphilos; j++)
    {
      sem[j] = semcreate (0);

    }

  intmask mask = disable ();

  int identity;
  for (identity; identity < nphilos; identity++)
    {
      resume(create(every_philosopher, 1024, 20, "Philosopher", 1, identity));
	  temp++ ;
    }

  restore (mask);



  if(temp <= ncycles)
  {
	  return OK;
}


}


void
every_philosopher (int i)
{
  while (TRUE)
    {
      thinking (i);
      pickup (i);
      eating (i);
      putit_back (i);
    }
}

void
pickup (int i)
{
  wait (mutex);
  state[i] = 1;
  kprintf (" \n This philosopher is hungry : %d", i);
  testing (i);
  signal (mutex);
  wait (sem[i]);
}

void
putit_back (int i)
{
  wait (mutex);
  state[i] = 0;
  kprintf (" \n Philosopher puts the forks down : %d", i);
  testing (RIGHT);
  testing (LEFT);
  signal (mutex);
}


void
testing (int i)
{
  if (state[i] == 1 && state[LEFT] != 2 && state[RIGHT] != 2)
    {
      kprintf (" \n Philosopher is eating : %d ", i);

      state[i] = 2;
      signal (sem[i]);
    }
}

void
thinking (int i)
{
  kprintf ("\n Philosopher is thinking : %d", i);
  rand_delay (maxdelay);
}

void
eating (int i)
{
  kprintf (" \n Philosopher is eating : %d ", i);
  rand_delay (maxdelay);
}
