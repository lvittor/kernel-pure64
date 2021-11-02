#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <stdlib.h>
#include <lib.h>
#include <keyboard.h>
#include <video.h>
#include <scheduler.h>
#include <memoryManager.h>
#include <semaphore.h>
#include <pipes.h>
#include <time.h>

#define MAX_MEMORY_MAPPED 0x1000000000
#define MAX_SYSCALLS 22

typedef struct dateType {
	uint8_t year, month, day;
	uint8_t hour, minute, second;
} dateType;

uint64_t sys_exit(void);
uint64_t sys_write(uint8_t fd, char * buffer, uint64_t count);
int64_t sys_read(int fd);
uint8_t sys_getpid(void);
void sys_processlist(void);
uint8_t sys_createProcess(processPrototype * pPP, int argc, char* argv[]);
uint64_t sys_kill(uint8_t pid);
uint64_t sys_block(uint8_t pid);
void * sys_alloc(size_t size);
void sys_free(void * address);
void sys_mem_dump(void);
int sys_nice(pid_t pid, priority_t priority);
SEM_RET sys_open_sem(semid_t sid, semvalue_t value);
SEM_RET sys_wait_sem(semid_t sid);
SEM_RET sys_post_sem(semid_t sid);
SEM_RET sys_close_sem(semid_t sid);
void sys_print_sem(void);
int sys_open_pipe(int fd[2]);
int sys_write_pipe(int fd, char * buffer, int count);
int sys_read_pipe(int fd);
int sys_close_pipe(int fd);
int sys_secondsElapsed(void);

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
	(syscall)sys_open_sem,
	(syscall)sys_wait_sem,
	(syscall)sys_post_sem,
	(syscall)sys_close_sem,
	(syscall)sys_print_sem,
	(syscall)sys_open_pipe,
	(syscall)sys_write_pipe,
	(syscall)sys_read_pipe,
	(syscall)sys_close_pipe,
	(syscall)sys_secondsElapsed,
};

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	syscall sc;
	if (rcx >= MAX_SYSCALLS || (sc = syscalls[rcx]) == NULL)
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
	if (buffer == NULL || count <= 0)
		return -1;
	if (fd < 3)
		fd = getMappedFDFromProcess(getCurrentPID(), fd);
	
	if (fd == STD_OUT || fd == STD_ERR) {
		Color * fontColor = (fd == STD_ERR) ? &RED : &WHITE;
		for (int i = 0; i < count && buffer[i]; i++)
			ncPrintCharAtt(buffer[i], fontColor, &BLACK);
		return count;
	}
	
	return writePipe(fd, buffer, count);
}

int64_t sys_read(int fd) {
	if (fd < 3)
		fd = getMappedFDFromProcess(getCurrentPID(), fd);
	if (fd == STD_IN)
  		return getChar();
	return readPipe(fd);
}

void sys_processlist(void) {
	printProcesses();
}

uint8_t sys_createProcess(processPrototype * pPP, int argc, char* argv[]) {
	return loadProcess(pPP, argc, argv);
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

SEM_RET sys_open_sem(semid_t sid, semvalue_t value) {
	return openSemaphore(sid, value);
}

SEM_RET sys_wait_sem(semid_t sid) {
	return waitSemaphore(sid);
}

SEM_RET sys_post_sem(semid_t sid) {
	return postSemaphore(sid);
}

SEM_RET sys_close_sem(semid_t sid) {
	return closeSemaphore(sid);
}

void sys_print_sem(void) {
	printSemaphores();
}

int sys_open_pipe(int fd[2]) {
	return openPipe(fd);
}

int sys_write_pipe(int fd, char * buffer, int count) { 
	return writePipe(fd, buffer, count);
}

int sys_read_pipe(int fd) {
	return readPipe(fd);
}

int sys_close_pipe(int fd) {
	return closePipe(fd);
}

int sys_secondsElapsed(void){
	return seconds_elapsed();
}
