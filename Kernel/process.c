#include <process.h>
#include <scheduler.h>
#include <mmgr.h>
#include <interrupts.h>

#define MAX_PROCESS_COUNT 256
#define PROCESS_SIZE 8 * 1024

typedef struct process {
    pid_t pid;
    uint8_t priority;
    Status status;
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
    newProcess->status = READY;
    newProcess->rip = rip;
    newProcess->priority = priority;
    processes[processCounter++] = newProcess;

    return newProcess->pid;
}

void kill(pid_t pid) {
    if(processes[pid] != NULL){
        processes[pid]->status = TERMINATED;
        checkCurrent(pid);
    }
}

void block(pid_t pid){
    if(processes[pid] != NULL){
        processes[pid]->status = BLOCKED;
        checkCurrent(pid);
    }
}

void unblock(pid_t pid){
    if(processes[pid] != NULL && processes[pid]->status == BLOCKED){
        processes[pid]->status = READY;
    }
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

uint8_t isReady(pid_t pid) {
    return processes[pid] == NULL ? 0 : processes[pid]->status == READY;
}

uint8_t isBlocked(pid_t pid) {
    return processes[pid] == NULL ? 0 : processes[pid]->status == BLOCKED;
}

uint8_t isTerminated(pid_t pid) {
    return processes[pid] == NULL ? 1 : processes[pid]->status == TERMINATED;
}


void setRsp(pid_t pid, uint64_t rsp) {
    if(pid < 0 || processes[pid] == NULL)
        return;
    processes[pid]->rsp = rsp;
}

void showAllPs() {
    for(int i = 0; i < MAX_PROCESS_COUNT; i++) {
        if(processes[i] != NULL) {
            ncPrint("PID: ");
            ncPrintDec(processes[i]->pid);
            ncNewline();
            ncPrint("Current rsp: ");
            ncPrintHex(processes[i]->rsp);
            ncNewline();
            ncPrint("Priority: ");
            ncPrintDec(processes[i]->priority);
            ncNewline();
            ncNewline();
        }
    }
}
