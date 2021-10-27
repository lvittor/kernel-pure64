#ifndef __SYNC_H__
#define __SYNC_H__

#include <stdint.h>
#include <stddef.h>

int sem_open(uint8_t id, uint64_t value);

int sem_wait(uint8_t semID);

int sem_post(uint8_t semID);

int sem_close(uint8_t semID);

#endif
