#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include <stdint.h>

#define MAX_SEMAPHORES  32

enum SEM_RET {
    SEM_SUCCESS = 0,
    SEM_EXISTS,
    SEM_INVALID,
    SEM_ENOMEM,
};

typedef char lock_t;
typedef uint8_t semid_t;
typdef uint64_t semvalue_t;

// C


// ASM
void _acquire(lock_t *lock);
void _release(lock_t *lock);


#endif