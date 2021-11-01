#include <scheduler.h>
#include <memoryManager.h>
#include <interrupts.h>
#include <naiveConsole.h>

enum processState {
    READY = 0,
    BLOCKED,
    ERROR,
    KILLED
} processState;

typedef struct processControlBlock {
    enum processState state;
    uint64_t functionAddress;
    uint64_t currRSP;
    uint64_t stackTop;
    uint64_t baseRSP;
} processControlBlock;

static processControlBlock * processes[MAX_PROCESSES];
static uint8_t currentPID = 0;
static uint8_t haltPID = 0;

void freeProcess(uint8_t pid);

void haltProcess(void) {
    while (1) {
        _hlt();
    }    
}

int8_t initScheduler(uint64_t functionAddress, int argc, char* argv[]) {
    haltPID = loadProcess((uint64_t)haltProcess, 0, (char *[]){NULL});
    if (haltPID == -1)
        return -1;
    
    int newPID = loadProcess(functionAddress, argc, argv);
    if (newPID == -1)
        return -1;
    
    currentPID = newPID;
    _openProcessContext(processes[currentPID]->currRSP);
    return 0;
}

int loadProcess(uint64_t functionAddress, int argc, char* argv[]) {
    int pid = 0;
    for (; pid < MAX_PROCESSES && processes[pid] != NULL; pid++);
    if (pid == MAX_PROCESSES)
        return -1;

    processControlBlock * pPCB = alloc(sizeof(processControlBlock));
    if (pPCB == NULL)
        return -1;   
    uint64_t stackTop = (uint64_t)alloc(PROCESS_STACK_SIZE);
    if (stackTop == (uint64_t)NULL) {
        free(pPCB);
        return -1;
    }
    pPCB->stackTop = stackTop;
    pPCB->state = READY;
    pPCB->baseRSP = stackTop + PROCESS_STACK_SIZE - 1;
    pPCB->functionAddress = functionAddress;
    pPCB->currRSP = _buildProcessContext(pPCB->baseRSP, pPCB->functionAddress, argc, argv);

    processes[pid] = pPCB;
    return pid;
}

uint64_t schedule(uint64_t currRSP) {
    processes[currentPID]->currRSP = currRSP;
    for (uint8_t i = 1; i < MAX_PROCESSES; i++) {
        uint8_t pid = (currentPID + i) % MAX_PROCESSES;
        if (processes[pid]->state == READY) {
            currentPID = pid;
            return processes[currentPID]->currRSP;
        } else if (processes[pid]->state == KILLED) {
            freeProcess(pid);
        }
        // else {
        //     Blocked... 
        // }
    }
    return processes[haltPID]->currRSP;
}

void freeProcess(uint8_t pid) {
    free((void*)processes[pid]->stackTop);
    free((void*)processes[pid]);
    processes[pid] = NULL;
}

int8_t getCurrentPID(void) {
    return currentPID;
}

static int validPID(uint8_t pid){
    return pid < MAX_PROCESSES && pid != haltPID;
}

int kill(uint8_t pid) {
    if(! validPID(pid))
        return -1;

    processes[pid]->state = KILLED;
    if (pid == getCurrentPID()) {
        _openProcessContext(schedule(0)); // Maybe: getCurrentRSP
        while(1);
    }
    return 0;
}

int block(uint8_t pid){
    if(! validPID(pid))
        return -1;

    if(processes[pid]->state == BLOCKED)
        processes[pid]->state = READY;
    else if(processes[pid]->state == READY) 
        processes[pid]->state = BLOCKED; 
    
    return 0;
}

processControlBlock * getProcesses(void) {
    ncPrint("\nProcess list:\n");
    for (int pid = 0; pid < MAX_PROCESSES; pid++){
        if (processes[pid] != NULL) {
            ncPrint("Process ");
            ncPrintDec(pid);
            ncPrint(": ");
            ncPrint("STATE: ");
            ncPrintDec(processes[pid]->state);
            ncPrintChar('\n');
        }
    }

    return NULL;
}