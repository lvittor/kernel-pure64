#ifndef __MM_H__
#define __MM_H__

#include <stdint.h>
#include <stdlib.h>

#define TOTAL_HEAP_SIZE 0x8000000
#define HEAP_START 0x600000

void heap_init(void);
void *alloc(size_t size);
void free(void *address);
void dump_mem(void);

#endif