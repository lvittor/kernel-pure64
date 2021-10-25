#include <stdint.h>

typedef int16_t pid_t;

// ASM
uint64_t init_process(uint64_t stack_base, uint64_t rip);
void run_process(uint64_t rsp);


int8_t saveProcess(uint64_t rip, uint8_t priority);
void kill(pid_t pid);
void remove(pid_t pid);
void setRsp(pid_t pid, uint64_t rsp);
uint64_t getRsp(pid_t pid);
uint8_t getPriority(pid_t pid);
uint8_t isAlive(pid_t pid);
void showAllPs();