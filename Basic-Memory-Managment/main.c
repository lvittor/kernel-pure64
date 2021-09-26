#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "mmgr.h"

int main() {
    initMgr();
    char *str = alloc(0);
    assert(str == NULL);
    str = alloc(8192);
    assert(str == NULL);
    str = alloc(20);
    uint64_t address = (uint64_t) str;
    strcpy(str, "Hola Mundo!");
    assert(strcmp(str, "Hola Mundo!") == 0);
    free(str);
    str = alloc(20);
    assert((uint64_t) str == address);
    puts("OK");
    return 0;
}