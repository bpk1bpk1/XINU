#ifndef PROCESS_RING_H
#define PROCESS_RING_H

// declare variables to store command line data
int rounds;
int processes;
enum versions { WORK, HANG, LOOP, CHAOS};
int version;

// these will be accessed/modified by lots of process
volatile int count;
volatile int round;

#endif
