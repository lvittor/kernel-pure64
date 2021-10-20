#include <process.h>
#include <mmgr.h>

#define MAX_PROCESS_COUNT 256
#define ALIVE 1
#define TERMINATED 0

typedef struct process {
    uint8_t pid, status, priority;
    uint64_t rsp;
} Process;

static Process * processes[MAX_PROCESS_COUNT];
static uint8_t processCounter = 0;

int saveProcess(uint64_t rsp, uint8_t priority) {
    Process *newProcess = alloc(sizeof(Process));
    if(newProcess == NULL){
        return -1;
    }

    newProcess->pid = processCounter;
    newProcess->status = ALIVE;
    newProcess->rsp = rsp;
    newProcess->priority = priority;
    processes[processCounter++] = newProcess;
    return 1;
}

