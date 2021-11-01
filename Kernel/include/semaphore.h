#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <stdint.h>

#define MAX_SEMAPHORES  32

typedef enum SEM_RET {
    SEM_SUCCESS = 0,
    SEM_EXISTS,
    SEM_INVALID,
    SEM_ENOMEM,
} SEM_RET;

typedef char lock_t;
typedef char * semid_t;
typedef uint64_t semvalue_t;

// ASM
void _acquire(lock_t *lock);
void _release(lock_t *lock);

// C
SEM_RET openSemaphore(semid_t sid, semvalue_t value);
SEM_RET waitSemaphore(semid_t sid);
SEM_RET postSemaphore(semid_t sid);
SEM_RET closeSemaphore(semid_t sid);

#endif