/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

#define MAXBUF 255
#define MAXMSG 255
#define MAXMAXDELAY 5000
#define MINALL 1
#define REQNARGS 4

/*------------------------------------------------------------------------
 * xsh_date - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_producer_consumer(int nargs, char *args[]) {

  // input variable declarations
  int bufsize;  // in bytes
  int msgsize;  // in bytes
  int maxdelay; // in miliseconds

  // make sure the correct number of arguments were passed or set sensible defaults
  if (nargs == 1) {
    bufsize = 100;
    msgsize = 10;
    maxdelay = 1000;
  } else if (nargs > REQNARGS) {
    kprintf("Error, too many arguments. Expected %d.\n", REQNARGS-1); return 1;
  } else if (nargs < REQNARGS) {
    kprintf("Error, too few arguments. Expected %d.\n", REQNARGS-1); return 1;
  } else {
    // validate the buffer size
    bufsize = atoi(args[1]);
    if (bufsize > MAXBUF || bufsize < MINALL) {
      kprintf("Error, invalid buffer size: %d\n", bufsize); return 1;
    }  
    // validate the message size
    msgsize = atoi(args[2]);
    if (msgsize > MAXMSG || msgsize < MINALL) {
      kprintf("Error, invalid message size: %d\n", msgsize); return 1;
    }  
    // validate the max delay
    maxdelay = atoi(args[3]);
    if (maxdelay > MAXMAXDELAY || maxdelay < MINALL) {
      kprintf("Error, invalid max delay: %d\n.", maxdelay); return 1;
    }  
  }

  kprintf("Buffer Size: %d\tMessage Size: %d\tMax Delay: %d\n", bufsize, msgsize, maxdelay);

  return 0;
}
