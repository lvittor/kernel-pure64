#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include <process.h>

void scheduler();
uint64_t get_rsp(void);
void run_process(uint64_t rsp);
int8_t initScheduler();
int8_t addToReady(pid_t proc);
int8_t addToBlocked(pid_t proc);

#endif