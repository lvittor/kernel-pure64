#ifndef MY_PROCESS_H
#define MY_PROCESS_H

#include <stdint.h>

typedef int16_t pid_t;

void ps(void);
pid_t createPs(uint64_t rip, char *name, int argc, char *argv[], uint8_t mode);
int block(pid_t pid);
int unblock(pid_t pid);
int kill(pid_t pid);
pid_t getpid(void);
int nice(pid_t pid, int adjustment);
void exit();
void yield(void);

#endif
