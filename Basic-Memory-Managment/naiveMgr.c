#include "mmgr.h"
#define MEM_SIZE 1024

static mptr mem = {0};
static char to_alloc[1024];

int initMgr(void) {
    mem.base = to_alloc;
    mem.top = (char *) mem.base + MEM_SIZE;
    mem.current = mem.base;
    return 0;
}

void *alloc(size_t bytes){
    if (bytes == 0 || (size_t) ((char *) mem.current + bytes) > (size_t) mem.top)
        return NULL;
    
    void *tmp = mem.current;
    mem.current = (char *) mem.current + bytes;
    return tmp;
}
 
void free(void *ptr){
    // Jaja muy bueno!
}
