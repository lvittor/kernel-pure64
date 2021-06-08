#ifndef __MULTITASKING_H_
#define __MULTITASKING_H_

#include <stdint.h>

uint64_t create_task(uint64_t id, uint64_t baseRSP, uint64_t function);
void init_task(uint64_t RSP);
void setRSP(uint64_t newRSP);
uint64_t getRSP(void);

#endif