#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <stdlib.h>
#include <lib.h>
#include <keyboard.h>
#include <video.h>
#include <multiprocess.h>

#define MAX_MEMORY_MAPPED 0x1000000000
#define MAX_SYSCALLS 15

typedef struct dateType {
	uint8_t year, month, day;
	uint8_t hour, minute, second;
} dateType;

uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count);
int64_t sys_read(void);
uint64_t sys_date(dateType * pDate);
uint64_t sys_mem(uint64_t rdi, uint64_t rsi, uint8_t rdx);
uint8_t sys_getpid(void);

typedef int64_t (*syscall)(int64_t, int64_t, int64_t);

syscall syscalls[MAX_SYSCALLS] = {
	(syscall)sys_getpid,
	(syscall)sys_write,
	(syscall)sys_read,
	(syscall)sys_date,
	(syscall)sys_mem,
	NULL
};

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	syscall sc = syscalls[rcx];
	if (sc == NULL)
		return -1;
	
	return sc(rdi, rsi, rdx);
}

uint8_t sys_getpid(void) {
	return getCurrentPID();
}

uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count) {
	if (buffer == 0 || count <= 0)
		return -1;
		
	if (fd > 2)
		return -1;

	Color * fontColor = (fd == STD_ERR) ? &RED : &WHITE;
    
	for (int i = 0; i < count && buffer[i]; i++)
		ncPrintCharAtt(buffer[i], fontColor, &BLACK);
	
	return count;
}

int64_t sys_read(void) {
  	return getChar();
}

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
	uint8_t * src = (uint8_t *)rdi;
	uint8_t * dst = (uint8_t *)rsi;
	
	// qemu tiene 64GB mapeados en memoria, asi que en el emulador
	// incluso con sólo 512MB de memoria
	// Podés acceder a todas las direcciones hasta 0x1000000000 - 1
	if (src >= (uint8_t *)MAX_MEMORY_MAPPED || src - 1 + rdx >= (uint8_t *)MAX_MEMORY_MAPPED)
		return 1;

	uint8_t i;
	for (i = 0; i < rdx; i++) 
		dst[i] = src[i];
	
	return i;
}

uint8_t sys_listProcesses(){
	return getProcesses();
}