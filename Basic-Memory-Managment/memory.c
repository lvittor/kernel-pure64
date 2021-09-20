#include "memory.h"

static h_mem my_mem = {0};

int initMemory(void) {
    my_mem.base = my_mem.memory;
    my_mem.top = (char *) my_mem.memory + MAX_MEM_LENGTH;
    my_mem.current = my_mem.base;
    return 0;
}

 
void *alloc(size_t bytes){
    if (bytes == 0 || (size_t) ((char *) my_mem.current + bytes) > (size_t) my_mem.top)
        return NULL;
    
    void *tmp = my_mem.current;
    my_mem.current = (char *) my_mem.current + bytes;
    return tmp;
}
 
void free(void *ptr){
    // Jaja muy bueno!
}
