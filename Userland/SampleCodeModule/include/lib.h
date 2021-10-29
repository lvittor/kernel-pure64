#ifndef __LIB_H__
#define __LIB_H__

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>

#define IN_RANGE(x, a, b) ((x) >= (a) && (x) <= (b))
#define ISUPPER(x) IN_RANGE(x, 'A', 'Z')
#define ISLOWER(x) IN_RANGE(x, 'a', 'z')
#define ISALPHA(x) (ISUPPER(x) || ISLOWER(x))
#define ISDIGIT(x) IN_RANGE(x, '0', '9')

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct dateType {
  uint8_t year, month, day;
  uint8_t hour, minute, second;
} dateType;

// C
int print_f(uint8_t fd, const char *format, ...);
void put_char(uint8_t fd, const char character);
void put_s(uint8_t fd, const char *s);
int sscan(char *buff, char *fmt, ...);
int64_t get_s(char *buffer, uint64_t maxLength);
int64_t strtoint(const char *nptr, char **endptr, register int base);
double strtodouble(const char *start, char **end);
int get_char(void);

// ASM
void print(uint8_t fd, const char *s, uint64_t count);
void setReg(void);
uint64_t fillDate(dateType *pDate);
void inforeg(void);
int64_t fillMem(uint64_t src, uint8_t *dst, uint8_t size);
long read(unsigned int fd, char *buf, size_t count);
int _quadratic(double *a, double *b, double *c, double *root1, double *root2);

#endif