#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx,
			   uint64_t rcx);

#endif