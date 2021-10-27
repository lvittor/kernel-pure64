#include <process.h>
#include <scheduler.h>
#include <mmgr.h>
#include <interrupts.h>
#include <kstring.h>
#include <naiveConsole.h>

#define MAX_PROCESS_COUNT 256
#define PROCESS_SIZE 8 * 1024

typedef struct process {
    pid_t pid;
    char *name;
    uint8_t priority;
    Status status;
    uint64_t rsp, rip, stack_base;
} Process;

static pid_t processCounter = 0;
static Process * processes[MAX_PROCESS_COUNT] = {NULL};
static char *states[] = {"Ready", "Terminated", "Blocked"};

static uint8_t isValidPid(pid_t pid) {
    return pid >= 0 && pid < MAX_PROCESS_COUNT && processes[pid] != NULL && processes[pid]->status != TERMINATED;
}

pid_t createProcess(uint64_t rip, uint8_t priority, char *name, uint64_t argc, char *argv[]) {
    Process *newProcess = alloc(sizeof(Process));
    
    if(newProcess == NULL){
        return -1;
    }

    newProcess->stack_base = (uint64_t) alloc(PROCESS_SIZE);
    uint64_t rsp = newProcess->stack_base + (PROCESS_SIZE - 1);

    newProcess->rsp = init_process(rsp, rip, argc, (uint64_t) argv);
    newProcess->pid = processCounter;
    newProcess->status = READY;
    newProcess->rip = rip;
    newProcess->priority = priority;
    newProcess->name = alloc(strlen(name) + 1);
    strcpy(newProcess->name, name);

    processes[processCounter++] = newProcess;

    addToReady(newProcess->pid);

    return newProcess->pid;
}

int kill(pid_t pid) {
    if(!isValidPid(pid))
        return -1;
    processes[pid]->status = TERMINATED;
    checkCurrent(pid);
    return 0;
}

int block(pid_t pid){
    if(!isValidPid(pid))
        return -1;
    processes[pid]->status = BLOCKED;
    checkCurrent(pid);
    return 0;
}

int unblock(pid_t pid){
    if(!isValidPid(pid))
        return -1;
    processes[pid]->status = READY;
    return 0;
}

void remove(pid_t pid) {
    if(isValidPid(pid)){
        free(processes[pid]->stack_base);
        free(processes[pid]->name);
        free(processes[pid]);
        processes[pid] = NULL;
    }
}

uint64_t getRsp(pid_t pid){
    return isValidPid(pid)? processes[pid]->rsp : 0;
}

int getPriority(pid_t pid) {
    if(!isValidPid(pid))
        return -1; 
    return processes[pid]->priority;
}

uint8_t isReady(pid_t pid) {
    return isValidPid(pid)? processes[pid]->status == READY : 0;
}

uint8_t isBlocked(pid_t pid) {
    return isValidPid(pid)? processes[pid]->status == BLOCKED : 0;
}

uint8_t isTerminated(pid_t pid) {
    return isValidPid(pid)? processes[pid]->status == TERMINATED : 1;
}


void setRsp(pid_t pid, uint64_t rsp) {
    if(isValidPid(pid))
        processes[pid]->rsp = rsp;
}

int setPriority(pid_t pid, uint8_t priority) {
    if(!isValidPid(pid))
        return -1;
    processes[pid]->priority = priority;
    return 0;
}

void showAllPs() {
    ncPrint("---------------------");
    ncNewline();
    for(int i = 0; i < MAX_PROCESS_COUNT; i++) {
        if(processes[i] != NULL) {
            ncPrint("PID: ");
            ncPrintDec(processes[i]->pid);
            ncNewline();
            ncPrint("Name: ");
            ncPrint(processes[i]->name);
            ncNewline();
            ncPrint("Current rsp: ");
            ncPrintHex(processes[i]->rsp);
            ncNewline();
            ncPrint("Priority: ");
            ncPrintDec(processes[i]->priority);
            ncNewline();
            ncPrint("Status: ");
            ncPrint(states[processes[i]->status]);
            ncNewline();
            ncPrint("---------------------");
            ncNewline();
        }
    }
}
