#ifndef __LIB_H__
#define __LIB_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

typedef struct dateType {
	uint8_t year, month, day;
	uint8_t hour, minute, second;
} dateType;

// C
int print_f(uint8_t fd, const char * format, ...);
void put_char(uint8_t fd, const char character);
void put_s(uint8_t fd, const char * s);
int scan (char * str, ...);
long strtol(const char *nptr, char **endptr, register int base);

// ASM
void print(uint8_t fd, char * s, uint64_t count);
void setReg(void);
uint64_t fillDate(dateType * pDate);
void inforeg(void);
int64_t fillMem(uint64_t src, uint8_t * dst, uint8_t size);
int64_t getChar(void);

#endif