#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

#include <stdint.h>

#define STD_OUT 1
#define STD_ERR 2
#define NULL (void *)0

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx,
                           uint64_t rax);

#endif