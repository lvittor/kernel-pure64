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
    ready = newQueue();
    if(ready == NULL) {
        return -1;
    }
    haltProcessPid = createProcess(&haltProcess, 0);
    pop(ready);
    return 0;
}

pid_t addToReady(pid_t pid) {
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

        uint8_t flag = 0;
        pid_t initialPid = currentPid;
        while(1) {
            if(isEmpty(ready) || (flag && initialPid == currentPid)) {
                currentPid = haltProcessPid;
                return getRsp(currentPid);
            }

            if(!flag) flag = 1;

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
        _int20();
    }
}

void haltProcess() {
    while(1) {
        _hlt();
    }
}
