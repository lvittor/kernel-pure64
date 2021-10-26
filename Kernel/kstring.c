#include <kstring.h>

uint64_t strlen(char *s){
    int i = 0;
    while(s[i++]);
    return i - 1;
}

void strcpy(char *s, char *t){
    while(*t != 0){
        *(s++) = *(t++);
    }
    *s = 0;
}
