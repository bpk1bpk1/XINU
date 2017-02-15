/* xsh_date.c - xsh_date */

#include <xinu.h>
#include <string.h>
#include <stdio.h>


#define MAXBUF 1000
#define MAXMSG 1000000
#define MAXMAXDELAY 10000
#define MINALL 1
#define REQNARGS 4

void producer(sid32 mutex, sid32 items, sid32 spaces, char buffer[MAXBUF]);
void consumer(sid32 mutex, sid32 items, sid32 spaces, char buffer[MAXBUF], sid32 isdone);

// input variable declarations
int bufsize;  // in bytes
int msgs;  // in bytes
int maxdelay; // in miliseconds

/*------------------------------------------------------------------------
 * xsh_date - obtain and print the current month, day, year, and time
 *------------------------------------------------------------------------
 */
shellcmd xsh_producer_consumer(int nargs, char *args[]) {

  // make sure the correct number of arguments were passed or set sensible defaults
  if (nargs == 1) {
    bufsize = 5;
    msgs = 5;
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
    msgs = atoi(args[2]);
    if (msgs > MAXMSG || msgs < MINALL) {
      kprintf("Error, invalid message size: %d\n", msgs); return 1;
    }  
    // validate the max delay
    maxdelay = atoi(args[3]);
    if (maxdelay > MAXMAXDELAY || maxdelay < MINALL) {
      kprintf("Error, invalid max delay: %d\n.", maxdelay); return 1;
    }  
  }

  kprintf("Buffer Size: %d\tMessages: %d\tMax Delay: %d\n", bufsize, msgs, maxdelay);

  sid32 isdone = semcreate(0);
  sid32 mutex = semcreate(1);
  sid32 items = semcreate(0);
  sid32 spaces = semcreate(bufsize);
  // create a clean buffer initialized to all null
  char buffer[MAXBUF]; int c;
  for (c = 0; c < MAXBUF; c++) { buffer[c] = (char) 0; }

  resume(create(producer, 1024, 20, "producer", 4, mutex, items, spaces, buffer));
  resume(create(consumer, 1024, 20, "consumer", 5, mutex, items, spaces, buffer, isdone));

  wait(isdone);
  
  return 0;
}

void producer(sid32 mutex, sid32 items, sid32 spaces, char buffer[MAXBUF]) {
  uint32 pi;
  uint32 pj;
  uint32 pm;
  for (pi = 0; pi < msgs; pi++) {

    rand_delay(maxdelay); // wait some random amount of time for a message
    pm = (char) pi % 255;
    kprintf("Producer: %d\n", pm);

    wait(spaces); // begin critical region
    wait(mutex); // begin critical region

    // push to buffer (FILO)
    for (pj = 0; pj < bufsize; pj++) {
      if (buffer[pj] == (char) 0) {
	buffer[pj] = pm;
	// kprintf("push: buf[%d] = %d\n", pj, pm);
	break;
      }
    }

    signal(mutex); // end critical region
    signal(items);

  }
  kprintf("Producer: done\n");
  return;
}

void consumer(sid32 mutex, sid32 items, sid32 spaces, char buffer[MAXBUF], sid32 isdone) {
  uint32 ci = msgs;
  uint32 cj;
  uint32 cm;
  while (ci--) {

    wait(items);
    wait(mutex); // begin critical region

    // pop from buffer (FILO)
    for (cj = 0; cj < bufsize; cj++) {
      if (buffer[cj+1] == (char) 0) {
	cm = (uint32) buffer[cj];
	buffer[cj] = (char) 0;
	// kprintf("pop: buf[%d] = %d\n", cj, cm);
	break;
      }
    }

    signal(mutex); // end critical region
    signal(spaces);

    rand_delay(maxdelay); // spend some random amount of time processing this message
    kprintf("Consumer: %d\n", cm);
  }
  kprintf("Consumer: done\n");
  signal(isdone);
  return;
}

