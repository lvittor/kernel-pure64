#include <stdint.h>

typedef struct process {
    uint8_t pid, status, priority;
    uint64_t rsp;
} Process;

typedef uint8_t pid_t;

int8_t saveProcess(uint64_t rsp, uint8_t priority);
void kill(pid_t pid);
void remove(pid_t pid);
uint64_t getRsp(pid_t pid);
uint8_t getPriority(pid_t pid);
uint8_t isKilled(pid_t pid);