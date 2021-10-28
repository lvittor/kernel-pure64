#ifndef __MULTIPROCESS_H__
#define __MULTIPROCESS_H__

#include <stdint.h>

#define MAX_PROCESSES   128
#define PROCESS_STACK_SIZE  0x1000 // 4Kib

int loadProcess(uint64_t functionAddress, int argc, char * argv[]);
int8_t initScheduler(uint64_t functionAddress, int argc, char* argv[]);
void _openProcessContext(uint64_t baseRSP);
uint64_t _buildProcessContext(uint64_t baseRSP, uint64_t functionAddress, int argc, char * argv[]);

#endif