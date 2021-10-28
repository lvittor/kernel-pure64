#ifndef __LIB_H__
#define __LIB_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define IN_RANGE(x, a, b) ((x) >= (a) && (x) <= (b))
#define ISUPPER(x) IN_RANGE(x, 'A', 'Z')
#define ISLOWER(x) IN_RANGE(x, 'a', 'z')
#define ISALPHA(x) (ISUPPER(x) || ISLOWER(x))
#define ISDIGIT(x) IN_RANGE(x, '0', '9')

typedef struct dateType {
	uint8_t year, month, day;
	uint8_t hour, minute, second;
} dateType;

// C
int print_f(uint8_t fd, const char * format, ...);
void put_char(uint8_t fd, const char character);
void put_s(uint8_t fd, const char * s);
int sscan(char * buff, char * fmt, ...) ;
int64_t strtoint(const char *nptr, char **endptr, register int base);
double strtodouble(const char * start, char ** end);
int64_t get_s(char * buffer, uint64_t maxLength);

// ASM
void print(uint8_t fd, const char * s, uint64_t count);
void setReg(void);
uint64_t fillDate(dateType * pDate);
void inforeg(void);
int64_t fillMem(uint64_t src, uint8_t * dst, uint8_t size);
int64_t getChar(void);
int _quadratic(double * a, double * b, double * c, double * root1, double * root2);
uint8_t getPid(void);

#endif