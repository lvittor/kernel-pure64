#include <sync.h>
#include <queue.h>
#include <mmgr.h>
#include <assert.h>
#include <scheduler.h>
#include <process.h>

typedef struct semaphore {
    uint64_t value, activeCount;
    int64_t mutex;
    Queue blockedQueue;
} Semaphore;

static sem_t semaphores[MAX_SEMS] = {NULL};

sem_t sem_open(uint8_t id, uint64_t value) {
    if (semaphores[id]) {
        semaphores[id]->activeCount++;
        return semaphores + id;
    }
    semaphores[id] = alloc(sizeof(Semaphore));
    if (semaphores[id] == NULL) {
        return NULL;
    }
    semaphores[id]->value = value;
    semaphores[id]->activeCount = 1;
    semaphores[id]->mutex = 0;
    semaphores[id]->blockedQueue = newQueue();

    return semaphores[id];
}

int sem_wait(sem_t sem) {
    if (sem == NULL) {
        return -1;
    }
    if (sem->value > 0) {
        sem->value--;
        return 0;
    }
    pid_t currentPid = getCurrentPid();
    if (push(sem->blockedQueue, currentPid) == -1) {
        return -1;
    }
    block(currentPid);
    return 0;
}

int sem_post(sem_t sem) {
    if (sem == NULL) {
        return -1;
    }
    if (queueSize(sem->blockedQueue) == 0) {
        sem->value++;
        return 0;
    }
    pid_t toUnblockPid = pop(sem->blockedQueue);
    if (toUnblockPid == -1) {
        return -1;
    }
    unblock(toUnblockPid);
    return 0;
}

int sem_close(sem_t sem) {
    if (sem == NULL) {
        return -1;
    }
    sem->activeCount--;
    if (sem->activeCount > 0) {
        return 0;
    }
    semaphores[(uint8_t)(&sem - semaphores)] = NULL;
    free(sem);
    return 0;
}