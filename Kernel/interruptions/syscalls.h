#ifndef __SYSCALLS_H__
#define __SYSCALLS_H__

#include <stdint.h>

#define STD_OUT 1
#define STD_ERR 2

void sys_write(uint8_t fd, char * buffer, uint64_t count);

#endif