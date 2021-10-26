#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include <process.h>

int8_t initScheduler();
pid_t addToReady(uint64_t rip, uint8_t priority);
uint64_t scheduler(uint64_t rsp);
void checkCurrent(pid_t pid);

#endif