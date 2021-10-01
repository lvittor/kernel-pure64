#ifndef MEMORY_ADT_H
#define MEMORY_ADT_H

#include <stddef.h>

int initMgr(void);

void *alloc(size_t size);

void free(void *ptr);

#endif // MEMORY_ADT_H