#include <process.h>
#include <scheduler.h>
#include <mmgr.h>
#include <naiveConsole.h>

#define MAX_PROCESS_COUNT 256
#define PROCESS_SIZE 4 * 1024
#define TERMINATED 0x0
#define ALIVE 0x1

typedef struct process {
    pid_t pid;
    uint8_t status, priority;
    uint64_t rsp, rip;
} Process;

static pid_t processCounter = 0;
static Process * processes[MAX_PROCESS_COUNT] = {NULL};

int8_t saveProcess(uint64_t rip, uint8_t priority) {

    Process *newProcess = alloc(sizeof(Process));
    
    if(newProcess == NULL){
        return -1;
    }

    uint64_t rsp = (uint64_t) alloc(PROCESS_SIZE);
    rsp += (PROCESS_SIZE - 1);

    newProcess->rsp = init_process(rsp, rip);
    newProcess->pid = processCounter;
    newProcess->status = ALIVE;
    newProcess->rip = rip;
    newProcess->priority = priority;
    processes[processCounter++] = newProcess;

    return newProcess->pid;
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

void setRsp(pid_t pid, uint64_t rsp)    {
    if(processes[pid] != NULL)
        processes[pid]->rsp = rsp;
}