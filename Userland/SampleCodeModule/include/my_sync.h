#ifndef MY_SYNC_H
#define MY_SYNC_H

#include <stdint.h>

void sem_dump(void);

int sem_open(uint16_t semId, uint64_t value);

int sem_wait(uint16_t semId);

int sem_post(uint16_t semId);

int sem_close(uint16_t semId);

#endif
