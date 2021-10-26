#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

typedef int16_t pid_t;

typedef enum Status {
    READY = 0,
    TERMINATED,
    BLOCKED
} Status;

// ASM
uint64_t init_process(uint64_t stack_base, uint64_t rip);
void _int20(void);


int8_t saveProcess(uint64_t rip, uint8_t priority);
void kill(pid_t pid);
void block(pid_t pid);
void unblock(pid_t pid);
void remove(pid_t pid);
void setRsp(pid_t pid, uint64_t rsp);
uint64_t getRsp(pid_t pid);
uint8_t getPriority(pid_t pid);
uint8_t isReady(pid_t pid);
uint8_t isBlocked(pid_t pid);
uint8_t isTerminated(pid_t pid);
void showAllPs(void);

#endif