#ifndef __LIB_H__
#define __LIB_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

void print(uint8_t fd, char * s, uint64_t count);
void setReg(void);
int print_f(uint8_t fd, const char * format, ...);
void put_char(uint8_t fd, const char character);
void put_s(uint8_t fd, const char * s);

#endif