#include <scheduler.h>
#include <queue.h>
#include <process.h>
#include <stddef.h>

static Queue ready = NULL;
static pid_t currentPid;
static uint8_t remainingRuns;

int8_t initScheduler() {
    currentPid = -1;
    remainingRuns = 0;
    ready = newQueue();
    if(ready == NULL) {
        return -1;
    }
    return 0;
}

int8_t addToReady(uint64_t rip, uint8_t priority) {

    pid_t pid = saveProcess(rip, priority);

    if(pid < 0){
        return pid;
    }

    return push(ready, pid);
}

int8_t addToBlocked(pid_t proc) {
    // TODO
}

uint64_t scheduler(uint64_t rsp) {
    setRsp(currentPid, rsp);
    if(remainingRuns == 0){
        if(currentPid >= 0){
            push(ready, currentPid);
        }
        currentPid = pop(ready);
        remainingRuns = getPriority(currentPid);
    } else {
        remainingRuns--;
    }
    return getRsp(currentPid);
}
