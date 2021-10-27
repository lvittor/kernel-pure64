#ifndef __SYNC_H__
#define __SYNC_H__

#include <stdint.h>
#include <stddef.h>

#define MAX_SEMS 255 // 2^8 - 1

typedef struct semaphore * sem_t;

sem_t sem_open(uint8_t id, uint64_t value);

int sem_wait(sem_t sem);

int sem_post(sem_t sem);

int sem_close(sem_t sem);

#endif
