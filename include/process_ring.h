#ifndef PROCESS_RING_H
#define PROCESS_RING_H


#define MAXPROC 60
#define INT_MAX 2147483647


int rounds;
int processes;
enum versions { WORK, HANG, LOOP, CHAOS};
int version;


volatile int count;


volatile int variable1[MAXPROC];


void processring(int ring_element, sid32* proctrl, sid32 isdone);
void workingring (int ring_element, sid32* proctrl, sid32 isdone);
void chaosring (int ring_element, sid32 isdone);
void hangring (int ring_element);
void loopring (int ring_element);
void parse_argv_pair(char* key, char* val);
void printErr(char* msg);

#endif
