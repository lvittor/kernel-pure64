#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
int strlength(const char * s);
void strcopy(char * to, const char * from);
int strcmp(const char * a, const char * b);

char *cpuVendor(char *result);

uint8_t getYear();
uint8_t getMonth();
uint8_t getDay();
uint8_t getHour();
uint8_t getMinute();
uint8_t getSecond();
uint8_t getKey();

#endif