/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

#define MAXPROC 10
#define MAXCYCLES 10
#define MAXMAXDELAY 5000
#define MINALL 1
#define REQNARGS 6

/*------------------------------------------------------------------------
 * xsh_date - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_readers_writers(int nargs, char *args[]) {

  // input variable declarations
  int nWriters;
  int nReaders;
  int nWriteCycles;
  int nReadCycles;
  int maxdelay; // in miliseconds

  // make sure the correct number of arguments were passed or set sensible defaults
  if (nargs == 1) {
    nWriters = 3;
    nReaders = 3;
    nWriteCycles = 5;
    nReadCycles = 5;
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

  kprintf("Writers: %d\tReaders: %d\tWrite Cycles: %d\tRead Cycles: %d\tMax Delay: %d\n", nWriters, nReaders, nWriteCycles, nReadCycles, maxdelay);

  return 0;
}
