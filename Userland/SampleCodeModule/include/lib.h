#ifndef __LIB_H__
#define __LIB_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

typedef struct dateType {
	uint8_t year, month, day;
	uint8_t hour, minute, second;
} dateType;

void print(uint8_t fd, char * s, uint64_t count);
uint64_t fillDate(dateType * pDate);
void setReg(void);
int print_f(uint8_t fd, const char * format, ...);
void put_char(uint8_t fd, const char character);
void put_s(uint8_t fd, const char * s);
void printDate(void);
void inforeg(void);
void fillMem(uint64_t src, uint8_t * dst, uint8_t size);
//////////////
uint8_t strlen(const char *str);
int8_t strcmp (const char *p1, const char *p2);
//////////////
void help(void);
void printmem(void);

#endif