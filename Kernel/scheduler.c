#include <scheduler.h>
#include <memoryManager.h>
#include <interrupts.h>
#include <naiveConsole.h>

#define MAX_PROCESSES   128
#define PROCESS_STACK_SIZE  0x1000 // 4Kib

typedef enum processState {
    READY = 0,
    BLOCKED,
    ERROR,
    KILLED
} processState;

typedef struct processControlBlock {
    processState state;
    uint64_t functionAddress;
    uint64_t currRSP;
    uint64_t stackTop;
    uint64_t baseRSP;
    priority_t priority;
} processControlBlock;

static processControlBlock * processes[MAX_PROCESSES];
static pid_t currentPID = 0;
static pid_t haltPID = 0;
static uint8_t tickets = 0;

static void freeProcess(pid_t pid);
static uint8_t getTicketsForPID(pid_t pid);

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
    pPCB->priority = 0;

    processes[pid] = pPCB;
    return pid;
}

static char isValidPID(uint8_t pid){
    return pid < MAX_PROCESSES && pid != haltPID;
}

static char isPIDInState(pid_t pid, processState state) {
    return isValidPID(pid) && processes[pid]->state == state;
}

static char isSchedulablePID(pid_t pid) {
    return isPIDInState(pid, READY) && pid != haltPID;
}

static pid_t getNextReadyPID(pid_t currentPID) {
    for (pid_t step = 0; step < MAX_PROCESSES; step++) {
        pid_t nextPID = (currentPID + step + 1) % MAX_PROCESSES;
        if (isSchedulablePID(nextPID))
            return nextPID;
        if (isPIDInState(nextPID, KILLED))
            freeProcess(nextPID);
        // else BLOCKED OR ERROR
    }
    return haltPID;
}

uint64_t schedule(uint64_t currRSP) {
    processes[currentPID]->currRSP = currRSP;
    if (tickets == 0 || ! isPIDInState(currentPID, READY)) {
        currentPID = getNextReadyPID(currentPID);
        tickets = getTicketsForPID(currentPID);
    } else {
        tickets--;
    }
    return processes[currentPID]->currRSP;
}

void freeProcess(uint8_t pid) {
    free((void*)processes[pid]->stackTop);
    free((void*)processes[pid]);
    processes[pid] = NULL;
}

int8_t getCurrentPID(void) {
    return currentPID;
}
void yieldProcess(void) {
    _int20();
}

int kill(uint8_t pid) {
    if(! isValidPID(pid))
        return -1;

    processes[pid]->state = KILLED;
    if (pid == getCurrentPID()) {
        _openProcessContext(schedule(0)); // Maybe: getCurrentRSP
        while(1);
    }
    return 0;
}

int block(uint8_t pid){
    if(! isValidPID(pid))
        return -1;

    if(processes[pid]->state == BLOCKED)
        processes[pid]->state = READY;
    else if(processes[pid]->state == READY) 
        processes[pid]->state = BLOCKED; 
    
    return 0;
}

static char isValidPriority(priority_t priority) {
    return priority >= 0 && priority < MAX_PRIORITY;
}

int nice(pid_t pid, priority_t newPriority) {
    if (! isValidPID(pid))
        return -1;
    
    if (! isValidPriority(newPriority))
        return -1;
    
    processes[pid]->priority = newPriority;
    return 0;
}

static uint8_t getTicketsForPID(pid_t pid) {
    if (! isValidPID(pid))
        return 0;
    
    return processes[pid]->priority + 1;
}

void printProcesses(void) {
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

int setPIDState(uint8_t pid, processState newState) {
    if (! validPID(pid))
        return -1;
    processes[pid]->state = newState;
}