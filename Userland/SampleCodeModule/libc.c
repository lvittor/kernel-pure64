#include <stdio.h>
#include <stdarg.h>
#include <lib.h>
#include <stdint.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	// Calculate characters for each digit
	do {
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	// Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2){
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

// http://www.firmcodes.com/write-printf-function-c/
int print_f(uint8_t fd, const char * format, ...) {
    va_list arg;
    va_start(arg, format);
    
    int i;
    char buff[24];
    char * s;
    const char * traverse;
    
    for(traverse = format; *traverse != '\0'; traverse++) {
        while(*traverse != '%') {
            if(*traverse == '\0') {
                va_end(arg);
                return (traverse - format) / sizeof(traverse);
            }
            put_char(fd, *traverse);
            traverse++;
        }

        traverse++;

        switch(*traverse) {
            case 'c':
                i = va_arg(arg, int);
                put_char(fd, i);
                break;
            case 's':
                s = va_arg(arg, char *);
                put_s(fd, s);
                break;
            case 'd':
                i = va_arg(arg, int);
                if (i < 0) {
                    i = -i;
                    put_char(fd, '-');
                }
                if(uintToBase(i, buff, 10) > 0) put_s(fd, buff);
                break;
            case 'x':
                i = va_arg(arg, uint64_t);
                if(uintToBase(i, buff, 16) > 0) put_s(fd, buff);
                break;
            case '\0':
                va_end(arg);
                return (traverse - format) / sizeof(traverse);
        }

    }
    va_end(arg);
    return (traverse - format) / sizeof(traverse);
}

void put_char(uint8_t fd, const char character) {
    print(fd, &character, 1);
}

void put_s(uint8_t fd, const char * s) {
    while(*s) put_char(fd, *s++);
}

void printDate() {
	dateType currDate;
	fillDate(&currDate);
	print_f(1, "Date: %d/%d/%d\nTime: %d:%d:%d (UTC)\n", currDate.day, 
                                                         currDate.month, 
                                                         currDate.year + 2000, 
                                                         currDate.hour, 
                                                         currDate.minute, 
                                                         currDate.second);
}

/////////////USER_STRING.H/////////////

int8_t strcmp (const char *p1, const char *p2) {
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    } while (c1 == c2);
  return c1 - c2;
}

uint8_t strlen(const char *str) {
	const char *s;
	for (s = str; *s; ++s);
	return (s - str);
}

/////////////COMANDITOS RIKIS/////////////

void help() {
    print_f(1, "Los comandos disponibles son: help, inforeg, printmem, date, divisionByZero, invalidOpCode.\n");
}

void printmem() {
    uint8_t arr[32];
    print_f(1, "Ingrese la direccion a partir de la cual quiere leer: ");
    uint64_t dir;
    // Leer dir

    fillMem(0x200000FF, arr, 32);

    //512M = 512 * 2^20 = 2^29 = 1 1111 1111 1111 1111 1111 1111 1111 = 1FFFFFFF

    print_f(1, "Memoria a partir de 0x%x: ", dir);
    for (int i = 0; i < 32; i++) {
        print_f(1, "%xh, ", arr[i]);
    }
    print_f(1, "\n");
}
