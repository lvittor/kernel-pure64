#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "mmgr.h"

int main() {
    initMgr();
    char *str = alloc(0);
    assert(str == NULL);
    str = alloc(2048);
    assert(str == NULL);
    str = alloc(20);
    strcat(str, "Hola Mundo!");
    str[1024] = 5;
    assert(strcmp(str, "Hola Mundo!") == 0);
    str = alloc(1004);
    assert(str != NULL);
    str = alloc(1);
    assert(str == NULL);
    free(str);
    puts("OK");
    return 0;
}