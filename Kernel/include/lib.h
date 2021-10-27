#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *memset(void *destination, int32_t character, uint64_t length);
void *memcpy(void *destination, const void *source, uint64_t length);

char *cpuVendor(char *result);

uint8_t getYear();
uint8_t getMonth();
uint8_t getDay();
uint8_t getHour();
uint8_t getMinute();
uint8_t getSecond();
uint8_t getKey();

int64_t exchange(int64_t mutex, int64_t value);

void acquire(int *mutex);
void release(int *mutex);

#endif