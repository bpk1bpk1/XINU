/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int nphilos; //These are the number of philosophers
int ncycles; //These are the number of cycles

#define MAXPHILOS 10
#define MAXCYCLES 10
#define MAXMAXDELAY 5000
#define MINALL 1
#define REQNARGS 4

#define LEFT (i + nphilos - 1) % nphilos //Helps determine the person who is sitting on the left of the philosopher
#define RIGHT (i + 1) % nphilos //Helps determine the person who is sitting on right of the philosopher

void every_philosopher (int i); //This is what the philosophers do
void pickup (int i); //Pickup the forks to eat
void putit_back (int i); //Put the forks back for the other philosophers to eat

void thinking (int i); //Introduces the delay while thinking
void testing (int i); //Helps check the state of the philosophers to the left and the right of the particular philosopher
void eating (int i); //Introduces the delay while eating

sid32 mutex; //This is to control the interprocess communication
sid32 sem[10]; //This is a semaphore for each of the philosophers
int maxdelay; //Maximum delay that could be included
int state[10]; //The state of each philosopher. 0 - THINKING, 1 - HUNGRY, 2 - EATING
int cycle[10]; //The number of cycles that each philosopher has eaten.



/*------------------------------------------------------------------------
 *  * xsh_date - obtain and print the current month, day, year, and time
 *   *------------------------------------------------------------------------
 *    */
shellcmd xsh_dining_philosophers (int nargs, char *args[])
{
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

  kprintf ("Philosophers: %d\tCycles: %d\tMax Delay: %d\n", nphilos, ncycles,maxdelay);
  mutex = semcreate (1);
  int j;
  for (j = 0; j < nphilos; j++)
    {
      sem[j] = semcreate(1);

    }//Initialises the semaphores of every philosopher

  int h;
  for(h=0; h<nphilos; h++)
  {
	  cycle[h] = 1; //Initialising each of the cycles of the philosophers to 1
  }

  intmask mask = disable ();

  int identity;
  for (identity = 0; identity < nphilos; identity++)
    {
      resume(create(every_philosopher, 1024, 20, "Philosopher", 1, identity));
	  temp++ ;
    }

  restore (mask);
  int k;
  for(k = 0; k < nphilos; k++)
  {
	  state[k] = 0; //Each of the philosophers begin by thinking
 }

}


void every_philosopher (int i)
{
  while (cycle[i] <= ncycles) //Only if the cycle of eating is not done, allow the philosopher to enter
    {
      thinking(i);
      pickup (i);
      eating (i);
      putit_back (i);
    }
}

void pickup (int i) //This is to pick up the forks
{
  wait (mutex);
  state[i] = 1;
  kprintf (" \n Philosopher: %d is thinking in cycle: %d", i,cycle[i]);
  testing (i);
  signal (mutex);
  wait(sem[i]);
}

void putit_back (int i) //This is to put the forks back
{
  wait (mutex);
  state[i] = 0;
  //kprintf (" \n Philosopher puts the forks down : %d",i);
  testing (RIGHT);
  testing (LEFT);
  signal (mutex);
}


void testing (int i) //Testing the state of each of the philosophers on the left and right.
{
  if (state[i] == 1 && state[LEFT] != 2 && state[RIGHT] != 2)
    {
      kprintf (" \n Philosopher: %d is eating in cycle: %d ", i,cycle[i]);
      state[i] = 2;
	  cycle[i]++;
      signal(sem[i]);
    }
}

void thinking (int i) //Introducing the delay
{
 // kprintf ("\n Philosopher is thinking : %d", i);
  rand_delay(maxdelay);
}

void eating (int i) //For the delay
{
 // kprintf (" \n Philosopher is eating : %d ", i);
  rand_delay(maxdelay);
}
