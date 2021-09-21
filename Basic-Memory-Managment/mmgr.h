#ifndef MEMORY_ADT_H
#define MEMORY_ADT_H

#include <stddef.h>

typedef struct mptr {
    void *base;
    void *top;
    void* current;
} mptr;

int initMgr(void);

void *alloc(size_t size);

void free(void *ptr);

#endif // MEMORY_ADT_H