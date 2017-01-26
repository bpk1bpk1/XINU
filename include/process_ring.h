#ifndef PROCESS_RING_H
#define PROCESS_RING_H

// declare variables to store command line data
int rounds;
int processes;
enum versions { WORK, HANG, LOOP, CHAOS};
int version;

// this will be accessed/modified by multiple process
volatile int count;

// function prototypes
void processring (int pi, sid32 proctrl, sid32 isdone);
void chaosring (int pi, sid32 isdone);
void parse_argv_pair(char* key, char* val);
void printErr(char* msg);

// maximum accepted values for number of process/rounds
#define MAXPROC 60
#define MAXROUND 2147483647

#endif
