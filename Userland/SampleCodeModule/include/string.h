#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>

int8_t strcmp(const char *p1, const char *p2);
uint8_t strlen(const char *str);
int strdiv(char *str, char **other, char token);
char *strcat(char *dest, const char *src);
void strcopy(char *dest, char *src);

#endif