#include <assert.h>
#include <lib.h>
#include <mmgr.h>
#include <process.h>
#include <queue.h>
#include <scheduler.h>
#include <sync.h>

#define MAX_SEMS 255 // 2^8 - 1

typedef struct semaphore *sem_t;

typedef struct semaphore {
  uint64_t value, activeCount;
  int64_t mutex;
  Queue blockedQueue;
} Semaphore;

static sem_t semaphores[MAX_SEMS] = {NULL};

int sem_open(uint8_t id, uint64_t value) {
  if (semaphores[id]) {
    acquire(&(semaphores[id]->mutex));
    semaphores[id]->activeCount++;
    release(&(semaphores[id]->mutex));
    return 0;
  }
  semaphores[id] = alloc(sizeof(Semaphore));
  if (semaphores[id] == NULL) {
    return -1;
  }
  semaphores[id]->value = value;
  semaphores[id]->activeCount = 1;
  semaphores[id]->mutex = 0;
  semaphores[id]->blockedQueue = newQueue();

  return 0;
}

int sem_wait(uint8_t semID) {
  if (semaphores[semID] == NULL) {
    return -1;
  }
  if (semaphores[semID]->value > 0) {
    acquire(&(semaphores[semID]->mutex));
    semaphores[semID]->value--;
    release(&(semaphores[semID]->mutex));
    return 0;
  }
  pid_t currentPid = getCurrentPid();
  acquire(&(semaphores[semID]->mutex));
  int ans = push(semaphores[semID]->blockedQueue, currentPid);
  release(&(semaphores[semID]->mutex));
  if (ans < 0) {
    return -1;
  }
  block(currentPid);
  return 0;
}

int sem_post(uint8_t semID) {
  if (semaphores[semID] == NULL) {
    return -1;
  }
  if (queueSize(semaphores[semID]->blockedQueue) == 0) {
    acquire(&(semaphores[semID]->mutex));
    semaphores[semID]->value++;
    release(&(semaphores[semID]->mutex));
    return 0;
  }
  acquire(&(semaphores[semID]->mutex));
  pid_t toUnblockPid = pop(semaphores[semID]->blockedQueue);
  release(&(semaphores[semID]->mutex));
  if (toUnblockPid < 0) {
    return -1;
  }
  unblock(toUnblockPid);
  return 0;
}

int sem_close(uint8_t semID) {
  if (semaphores[semID] == NULL) {
    return -1;
  }
  acquire(&(semaphores[semID]->mutex));
  semaphores[semID]->activeCount--;
  release(&(semaphores[semID]->mutex));
  if (semaphores[semID]->activeCount > 0) {
    return 0;
  }
  free(semaphores[semID]);
  acquire(&(semaphores[semID]->mutex));
  semaphores[semID] = NULL;
  release(&(semaphores[semID]->mutex));
  return 0;
}