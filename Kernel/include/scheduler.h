#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include <process.h>

int8_t initScheduler();
int8_t addToReady(uint64_t rip, uint8_t priority);
int8_t addToBlocked(pid_t proc);
uint64_t scheduler(uint64_t rsp);

#endif