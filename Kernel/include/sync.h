#ifndef __SYNC_H__
#define __SYNC_H__

#include <stddef.h>
#include <stdint.h>

#define MAX_SEMS 765
#define MAX_USER_SEMS 256

int sem_open(uint16_t id, uint64_t value);

int sem_wait(uint16_t semID);

int sem_post(uint16_t semID);

int sem_close(uint16_t semID);

void sem_dump(void);

#endif
