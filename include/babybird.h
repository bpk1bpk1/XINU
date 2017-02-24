#include <xinu.h>
#include <string.h>
#include <stdio.h>
extern volatile int wormsLeft;
extern volatile int babyEnds;
extern int num_baby_birds;
extern int num_eat_worms;
extern int num_fetch_worms;
//This is to initialise the babybird with every baby that goes to eat the worm.
void babyBird(int id_bird);
void parentBird();
//These are the mutex variables and the conditional variables used in the
//program.
extern volatile mutex_t babybird;
extern volatile mutex_t parentbird; 
extern volatile mutex_t babybird1;
extern volatile cond_t babybird_cond;
extern volatile cond_t parentbird_cond;

