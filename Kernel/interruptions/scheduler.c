#include <scheduler.h>
#include <queue.h>
#include <process.h>
#include <stddef.h>
#include <interrupts.h>
#include <naiveConsole.h>

static Queue ready = NULL;
static pid_t currentPid;
static uint8_t remainingRuns;
static pid_t haltProcessPid = -1;

void haltProcess();

int8_t initScheduler() {
    currentPid = -1;
    remainingRuns = 0;
    ncNewline();
    ready = newQueue();
    if(ready == NULL) {
        return -1;
    }
    haltProcessPid = saveProcess(&haltProcess, 0);
    return 0;
}

pid_t addToReady(uint64_t rip, uint8_t priority) {

    pid_t pid = saveProcess(rip, priority);

    if(pid < 0){
        return pid;
    }

    if(push(ready, pid) < 0)
        return -1;
    return pid;
}

uint64_t scheduler(uint64_t rsp) {
    setRsp(currentPid, rsp);
    if(remainingRuns == 0 || !isReady(currentPid)){
        if(currentPid >= 0){
            push(ready, currentPid);
        }

        if(isEmpty(ready)) {
            ncNewline();
            ncPrint("Queue is empty");
            ncNewline();
            currentPid = haltProcessPid;
            return getRsp(currentPid);
        }

        while(1) {
            currentPid = pop(ready);
            if(isReady(currentPid))
                break;
            else if(isTerminated(currentPid))
                remove(currentPid);
            else
                push(ready, currentPid);
        }

        remainingRuns = getPriority(currentPid);
    } else {
        remainingRuns--;
    }
    return getRsp(currentPid);
}

void checkCurrent(pid_t pid) {
    if(pid == currentPid){
        ncNewline();
        ncPrint("Interrupting current process");
        ncNewline();
        _int20();
    }
}

void haltProcess() {
    while(1) {
        ncNewline();
        ncPrint("No process to run");
        ncNewline();
        _hlt();
    }
}
