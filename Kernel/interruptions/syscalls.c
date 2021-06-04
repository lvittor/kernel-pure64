#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <lib.h>

typedef struct dateType {
	uint8_t year, month, day;
	uint8_t hour, minute, second;
} dateType;


uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count);
void sys_read(uint8_t fd, char * buffer, uint64_t count);
uint64_t sys_date(dateType * pDate);
uint64_t sys_mem(uint64_t rdi, uint64_t rsi, uint8_t rdx);

// TODO: Usar un arreglo y no switch case
uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	switch(rcx) {
		case 1:
			return sys_write(rdi, rsi, rdx);
			break;
		case 3:
			return sys_date(rdi);
			break;
		case 4:
			return sys_mem(rdi, rsi, rdx);
			break;
	}
	return 0;
}

uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count) {
	if (buffer == 0 || count <= 0)
		return -1;
		
	if (fd > 2)
		return -1;

	char attribute = (fd == STD_ERR) ? RED_ON_BLACK : WHITE_ON_BLACK;
    
	for (int i = 0; i < count && buffer[i]; i++)
		ncPrintCharAtt(buffer[i], attribute);
	
	return count;
}

// https://github.com/codyjack/pintos-3/blob/master/userprog/syscall.c
// void sys_read(uint8_t fd, char * buffer, uint64_t count){
// 	for (int i = 0; i < count && buffer[i]; i++){
// 		char c = getchar();
// 		buffer[i] = c;
// 		if (c == EOF) return i;
// 	}
// 	return count;
// }

uint8_t BCDToDec(uint8_t bcd) {
	return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}

uint64_t sys_date(dateType * pDate){
	if (pDate == 0)
		return 0;
	pDate->year = BCDToDec(getYear());
	pDate->month = BCDToDec(getMonth());
	pDate->day = BCDToDec(getDay());
	pDate->hour = BCDToDec(getHour());
	pDate->minute = BCDToDec(getMinute());
	pDate->second = BCDToDec(getSecond());
	return 1;
}

uint64_t sys_mem(uint64_t rdi, uint64_t rsi, uint8_t rdx){
	uint8_t * src = rdi;
	uint8_t * dst = rsi;
	for (uint8_t i = 0; i < rdx; i++) {
		dst[i] = src[i];
	}
}
