#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>

#define MAX_PROCESSES   128
#define PROCESS_STACK_SIZE  0x1000 // 4Kib

typedef enum processState {
    READY = 0,
    BLOCKED,
    ERROR,
    KILLED
} processState;

int loadProcess(uint64_t functionAddress, int argc, char * argv[]);
int8_t initScheduler(uint64_t functionAddress, int argc, char* argv[]);
uint64_t schedule(uint64_t currRSP);
void _openProcessContext(uint64_t baseRSP);
uint64_t _buildProcessContext(uint64_t baseRSP, uint64_t functionAddress, int argc, char * argv[]);

int8_t getCurrentPID(void);
int kill(uint8_t pid);
int block(uint8_t pid);
int unblock(uint8_t pid);
struct processControlBlock * getProcesses(void);
int setPIDState(uint8_t pid, processState newState); // Use only inside kernel, does not check if should yield after kill or block

#endif