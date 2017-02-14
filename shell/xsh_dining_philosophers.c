/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

#define MAXPHILOS 10
#define MAXCYCLES 10
#define MAXMAXDELAY 5000
#define MINALL 1
#define REQNARGS 4

/*------------------------------------------------------------------------
 * xsh_date - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_dining_philosophers(int nargs, char *args[]) {

  // input variable declarations
  int nphilos;  // in bytes
  int ncycles;  // in bytes
  int maxdelay; // in miliseconds

  // make sure the correct number of arguments were passed or set sensible defaults
  if (nargs == 1) {
    nphilos = 10;
    ncycles = 10;
    maxdelay = 1000;
  } else if (nargs > REQNARGS) {
    kprintf("Error, too many arguments. Expected %d.\n", REQNARGS-1); return 1;
  } else if (nargs < REQNARGS) {
    kprintf("Error, too few arguments. Expected %d.\n", REQNARGS-1); return 1;
  } else {
    // validate the buffer size
    nphilos = atoi(args[1]);
    if (nphilos > MAXPHILOS || nphilos < MINALL) {
      kprintf("Error, invalid number of philosophers: %d\n", nphilos); return 1;
    }  
    // validate the message size
    ncycles = atoi(args[2]);
    if (ncycles > MAXCYCLES || ncycles < MINALL) {
      kprintf("Error, invalid number of cycles: %d\n", ncycles); return 1;
    }  
    // validate the max delay
    maxdelay = atoi(args[3]);
    if (maxdelay > MAXMAXDELAY || maxdelay < MINALL) {
      kprintf("Error, invalid max delay: %d\n.", maxdelay); return 1;
    }  
  }

  kprintf("Philosophers: %d\tCycles: %d\tMax Delay: %d\n", nphilos, ncycles, maxdelay);

  return 0;
}
