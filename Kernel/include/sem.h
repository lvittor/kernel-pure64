#ifndef __SEM_H__
#define __SEM_H__

#include <stdint.h>

#define MAX_SEMS 32

typedef enum sem_ret_t {
	SEM_SUCCESS = 0,
	SEM_EXISTS,
	SEM_INVALID,
	SEM_ENOMEM,
} sem_ret_t;

typedef char lock_t;
typedef char *semid_t;
typedef uint64_t semvalue_t;

void _acquire(lock_t *lock);
void _release(lock_t *lock);

sem_ret_t open_sem(semid_t sid, semvalue_t value);
sem_ret_t wait_sem(semid_t sid);
sem_ret_t post_sem(semid_t sid);
sem_ret_t close_sem(semid_t sid);
void dump_sems(void);

#endif