#ifndef __SYNC_H__
#define __SYNC_H__

#include <stddef.h>
#include <stdint.h>

int sem_open(uint16_t id, uint64_t value);

int sem_wait(uint16_t semID);

int sem_post(uint16_t semID);

int sem_close(uint16_t semID);

#endif
