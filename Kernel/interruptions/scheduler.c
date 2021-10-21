#include <scheduler.h>
#include <queue.h>
#include <process.h>
#include <stddef.h>

static Queue ready = NULL;

int8_t initScheduler() {
    ready = newQueue();
    if(ready == NULL) {
        return -1;
    }
    return 0;
}

int8_t addToReady(pid_t pid) {
    if (push(ready, pid) < 0) {
        return -1;
    }
    return 0;
}

int8_t addToBlocked(pid_t proc) {
    // TODO
}

void scheduler() {
    // save old state
    pid_t previous_pid = pop(ready);
    
}
