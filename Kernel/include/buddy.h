#ifndef __BUDDY_H__
#define __BUDDY_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define TOTAL_HEAP_SIZE   256

int buddyInit();
void * alloc(size_t desiredSize);
void free(void * memory);

#endif