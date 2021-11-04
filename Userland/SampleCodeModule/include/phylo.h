#ifndef __PHILO_H__
#define __PHILO_H__

#include <lib.h>

#define MAX_PHILOSOPHERS 10
#define MIN_PHILOSOPHERS 5

#define ADD_PHILOSOPHER_CHAR 'a'
#define REMOVE_PHILOSOPHER_CHAR 'r'
#define QUIT_PHILOSOPHER_CHAR 'q'

#define EATING_STR "   E   "
#define PHILOSOPHING_STR "   .   "
#define HUNGRY_STR "   o   "

#define EATING_TIME 5
#define PHILOSOPHING_TIME 3

typedef enum philo_state_t { EATING = 0, PHILOSOPHING, HUNGRY } philo_state_t;

void initPhilos(void);

#endif