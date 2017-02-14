/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

#define MAXBUF 255
#define MINBUF 1
#define MAXMSG 255
#define MINMSG 1
#define MAXMAXDELAY 5000
#define MINMAXDELAY 1

/*------------------------------------------------------------------------
 * xsh_date - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_producer_consumer(int nargs, char *args[]) {

  // input variable declarations
  int bufsize;  // in bytes
  int msgsize;  // in bytes
  int maxdelay; // in miliseconds

  // make sure the correct number of arguments were passed
  if (nargs > 4) {
    kprintf("Error, too many arguments. Expected 3.\n"); return 1;
  } else if (nargs < 4) {
    kprintf("Error, too few arguments. Expected 3.\n"); return 1;
  }

  // validate the bufsize
  bufsize = atoi(args[1]);
  if (bufsize > MAXBUF || bufsize < MINBUF) {
    kprintf("Error, invalid buffer size: %d\n", bufsize); return 1;
  }  

  // validate the msgsize
  msgsize = atoi(args[2]);
  if (msgsize > MAXMSG || msgsize < MINMSG) {
    kprintf("Error, invalid message size: %d\n", msgsize); return 1;
  }  

  // validate the maxdelay
  maxdelay = atoi(args[3]);
  if (maxdelay > MAXMAXDELAY || msgsize < MINMSG) {
    kprintf("Error, invalid max delay: %d\n.", maxdelay); return 1;
  }  

  kprintf("Buffer Size: %d\tMessage Size: %d\tMax Delay: %d\n", bufsize, msgsize, maxdelay);

  return 0;
}
