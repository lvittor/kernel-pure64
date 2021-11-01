#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

#include <stdint.h>
#include <stdlib.h>

#define TOTAL_HEAP_SIZE   0x8000000
#define HEAP_START 0x600000

void heapInit(void);
void * alloc(size_t size);
void free(void * address);
void memoryDump(void);

#endif