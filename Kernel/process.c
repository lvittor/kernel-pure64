#include <process.h>
#include <scheduler.h>
#include <mmgr.h>

#define MAX_PROCESS_COUNT 256
#define TERMINATED 0x0
#define ALIVE 0x1

typedef struct process {
    pid_t pid;
    uint8_t status, priority;
    uint64_t rsp;
} Process;

static pid_t processCounter = 0;
static Process * processes[MAX_PROCESS_COUNT] = {NULL};

int8_t saveProcess(uint64_t rsp, uint8_t priority) {
    Process *newProcess = alloc(sizeof(Process));
    if(newProcess == NULL){
        return -1;
    }

    newProcess->pid = processCounter;
    newProcess->status = ALIVE;
    newProcess->rsp = rsp;
    newProcess->priority = priority;
    processes[processCounter++] = newProcess;
    addToReady(newProcess);
    return 1;
}

void kill(pid_t pid) {
    if(processes[pid] != NULL)
        processes[pid]->status = TERMINATED;
}

void remove(pid_t pid) {
    if(processes[pid] != NULL){
        free(processes[pid]);
        processes[pid] = NULL;
    }
}

uint64_t getRsp(pid_t pid){
    return processes[pid] == NULL? 0 : processes[pid]->rsp;
}

uint8_t getPriority(pid_t pid) {
    return processes[pid] == NULL ? 0 : processes[pid]->priority;
}

uint8_t isAlive(pid_t pid) {
    return processes[pid] == NULL ? 0 : processes[pid]->status;
}
