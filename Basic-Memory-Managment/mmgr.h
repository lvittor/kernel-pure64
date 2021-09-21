#ifndef MEMORY_ADT_H
#define MEMORY_ADT_H

#include <stddef.h>

#define MAX_MEM_LENGTH 1024

typedef struct h_mem {
    void *base;
    void *top;
    void* current;
    char memory[MAX_MEM_LENGTH];
} h_mem;

int initMemory(void);

void *allocm(size_t size);

int setmem(void *s, int c, size_t n);

#endif // MEMORY_ADT_H