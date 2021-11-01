#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>

#define MAX_PROCESSES   128
#define PROCESS_STACK_SIZE  0x1000 // 4Kib
#define MAX_PRIORITY    40

typedef uint8_t pid_t;
typedef uint8_t priority_t;

void _int20(void);
void _openProcessContext(uint64_t baseRSP);
uint64_t _buildProcessContext(uint64_t baseRSP, uint64_t functionAddress, int argc, char * argv[]);
int loadProcess(uint64_t functionAddress, int argc, char * argv[]);
int8_t initScheduler(uint64_t functionAddress, int argc, char* argv[]);
uint64_t schedule(uint64_t currRSP);
int8_t getCurrentPID(void);
int kill(uint8_t pid);
int block(uint8_t pid);
int unblock(uint8_t pid);
int nice(uint8_t pid);
void printProcesses(void);

#endif