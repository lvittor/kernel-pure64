#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <stdlib.h>
#include <lib.h>
#include <keyboard.h>
#include <video.h>
#include <scheduler.h>
#include <memoryManager.h>

#define MAX_MEMORY_MAPPED 0x1000000000
#define MAX_SYSCALLS 13

typedef struct dateType {
	uint8_t year, month, day;
	uint8_t hour, minute, second;
} dateType;

uint64_t sys_exit(void);
uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count);
int64_t sys_read(void);
uint8_t sys_getpid(void);
void sys_processlist(void);
uint8_t sys_createProcess(uint64_t functionAddress, int argc, char* argv[]);
uint64_t sys_kill(uint8_t pid);
uint64_t sys_block(uint8_t pid);
void * sys_alloc(size_t size);
void sys_free(void * address);
void sys_mem_dump(void);
int sys_nice(pid_t pid, priority_t priority);

typedef int64_t (*syscall)(int64_t, int64_t, int64_t);

syscall syscalls[MAX_SYSCALLS] = {
	(syscall)sys_exit,
	(syscall)sys_write,
	(syscall)sys_read,
	(syscall)sys_getpid,
	(syscall)sys_processlist,
	(syscall)sys_createProcess,
	(syscall)sys_kill,
	(syscall)sys_block,
	(syscall)sys_alloc,
	(syscall)sys_free,
	(syscall)sys_mem_dump,
	(syscall)sys_nice,
	NULL
};

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	syscall sc = syscalls[rcx];
	if (sc == NULL)
		return -1;
	
	return sc(rdi, rsi, rdx);
}

uint64_t sys_exit(void) {
	return kill(getCurrentPID());
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

void sys_processlist(void) {
	printProcesses();
}

uint8_t sys_createProcess(uint64_t functionAddress, int argc, char* argv[]) {
	return loadProcess(functionAddress, argc, argv);
}

uint64_t sys_kill(uint8_t pid) {
	return kill(pid);
}

uint64_t sys_block(uint8_t pid) {
	return block(pid);
}

void * sys_alloc(size_t size) {
	return alloc(size);
}

void sys_free(void * address){
	return free(address);
}

void sys_mem_dump(void) { 
	return memoryDump();
}

int sys_nice(pid_t pid, priority_t priority) {
	return nice(pid, priority);
}