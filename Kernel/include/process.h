#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>

uint64_t get_rsp(void);
uint64_t init_process(uint64_t stack_base, uint64_t rip);
void run_process(uint64_t rsp);

#endif