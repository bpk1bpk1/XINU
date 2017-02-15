/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

#define MAXPROC 10
#define MAXCYCLES 10
#define MAXMAXDELAY 10000
#define MINALL 1
#define REQNARGS 6

void reader(int ri, int rc);
void writer(int wi, int wc);

int nkids;
int nreaders;
int maxdelay; // in miliseconds

sid32 roomEmpty;
sid32 mutex;
sid32 turnstile;
sid32 isdone;

/*------------------------------------------------------------------------
 * xsh_date - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_readers_writers(int nargs, char *args[]) {

  nreaders = 0;

  // input variable declarations
  int nWriters;
  int nReaders;
  int nWriteCycles;
  int nReadCycles;

  // make sure the correct number of arguments were passed or set sensible defaults
  if (nargs == 1) {
    nWriters = 3;
    nReaders = 3;
    nWriteCycles = 3;
    nReadCycles = 3;
    maxdelay = 1000;
  } else if (nargs > REQNARGS) {
    kprintf("Error, too many arguments. Expected %d.\n", REQNARGS-1); return 1;
  } else if (nargs < REQNARGS) {
    kprintf("Error, too few arguments. Expected %d.\n", REQNARGS-1); return 1;
  } else {
    // validate the number of writers
    nWriters = atoi(args[1]);
    if (nWriters > MAXPROC || nWriters < MINALL) {
      kprintf("Error, invalid number of writers: %d\n", nWriters); return 1;
    }  
    // validate the number of readers
    nReaders = atoi(args[2]);
    if (nReaders > MAXPROC || nReaders < MINALL) {
      kprintf("Error, invalid number of readers: %d\n", nReaders); return 1;
    }  
    // validate the number of write cycles
    nWriteCycles = atoi(args[3]);
    if (nWriteCycles > MAXCYCLES || nWriteCycles < MINALL) {
      kprintf("Error, invalid number of write cycles: %d\n", nWriteCycles); return 1;
    }  
    // validate the number of read cycles
    nReadCycles = atoi(args[4]);
    if (nReadCycles > MAXCYCLES || nReadCycles < MINALL) {
      kprintf("Error, invalid number of read cycles: %d\n", nReadCycles); return 1;
    }  
    // validate the maxdelay
    maxdelay = atoi(args[5]);
    if (maxdelay > MAXMAXDELAY || maxdelay < MINALL) {
      kprintf("Error, invalid max delay: %d\n.", maxdelay); return 1;
    }  
  }

  kprintf("Writers: %d\tReaders: %d\tWrite Cycles: %d\tRead Cycles: %d\tMax Delay: %d\n\n", nWriters, nReaders, nWriteCycles, nReadCycles, maxdelay);

  // initialize semophores
  roomEmpty = semcreate(1);
  mutex = semcreate(1);
  turnstile = semcreate(1);
  nkids = nWriters + nReaders;

  for ( ; nWriters > 0; nWriters--) {
    resume(create(writer, 1024, 20, "writer", 2, nWriters, nWriteCycles));
  }

  for ( ; nReaders > 0; nReaders--) {
    resume(create(reader, 1024, 20, "reader", 2, nReaders, nReadCycles));
  }

  chprio(getpid(), 19); // lower this processes' priority to minimize busywaiting
  while(nkids>0) { sleep(0.5); } // wait until the number of child processes running is zero
  return 0;
}


void reader(int ri, int rc) {

  for ( ; rc >= 0; rc--) {
    // pass through the turnstile
    wait(turnstile);
    signal(turnstile);

    // increment the number of readers one at a time
    wait(mutex);
    if (++nreaders == 1) {
      wait(roomEmpty); // first reader in needs to wait until the room is empty
    }
    signal(mutex);

    rand_delay(maxdelay); // Do our reading task for a random amount of time
    kprintf("Reader %d\t%d\n", ri, rc);

    // decrement the number of readers one at a time
    wait(mutex);
    if (--nreaders == 0) {
      signal(roomEmpty); // last reader out needs to signal that the room is empty
    }
    signal(mutex);
  }
  nkids--;
  return;
}

void writer(int wi, int wc) {
  for ( ; wc >= 0; wc--) {
    
    wait(turnstile); // try to pass through the turnstile
    wait(roomEmpty); // block readers if the room isn't empty

    rand_delay(maxdelay); // do our writing task for some random amount of time
    kprintf("Writer %d\t%d\n", wi, wc);

    signal(turnstile); // finish passing throug the turnstile
    signal(roomEmpty); // the room is empty when a writer leaves
  }
  nkids--;
  return;
}

