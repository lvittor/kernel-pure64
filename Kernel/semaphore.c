#include <semaphore.h>
#include <stddef.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <queue.h>

static lock_t semaphoresLock = 0;
struct semaphoreCDT * semaphores[MAX_SEMAPHORES] = {NULL};

typedef struct semaphoreCDT {
  lock_t lock;
  uint8_t creatorProcess;
  semvalue_t value;
  queueADT waitingQueue;
} semaphoreCDT;

static char isValidSemaphoreID(semid_t sid) {
  return sid >= 0 && sid < MAX_SEMAPHORES;
}

static char isActiveSemaphoreID(semid_t sid) {
  return isValidSemaphoreID(sid) && semaphores[sid] != NULL;
}

SEM_RET openSemaphore(semid_t sid, semvalue_t value) {
  if (!isValidSemaphoreID(sid))
    return SEM_INVALID;
  
  _acquire(&semaphoresLock);
  
  if (isActiveSemaphoreID(sid)) {
    _release(&semaphoresLock);
    return SEM_EXISTS;
  }
  
  semaphores[sid] = alloc(sizeof(semaphoreCDT));
  if (semaphores[sid] == NULL) {
    _release(&semaphoresLock);
    return SEM_ENOMEM;
  }

  semaphores[sid]->waitingQueue = newQueue();
  if (semaphores[sid]->waitingQueue == NULL) {
    free(semaphores[sid]);
    _release(&semaphoresLock);
    return SEM_ENOMEM;
  }

  semaphores[sid]->lock = 0;
  semaphores[sid]->value = value;
  semaphores[sid]->creatorProcess = getCurrentPID();
  _release(&semaphoresLock);
  return SEM_SUCCESS;
}

SEM_RET waitSemaphore(semid_t sid) {
  if (!isValidSemaphoreID(sid))
    return SEM_INVALID;
  
  _acquire(&semaphoresLock);
  if (!isActiveSemaphoreID(sid)) {
    _release(&semaphoresLock);
    return SEM_INVALID;
  }
  _acquire(&(semaphores[sid]->lock));
  _release(&semaphoresLock);

  if (semaphores[sid]->value <= 0){
    setPIDState(getCurrentPID(), BLOCKED);
    enqueue(semaphores[sid]->waitingQueue, getCurrentPID());
    _release(&(semaphores[sid]->lock));
    yieldProcess();
    _acquire(&(semaphores[sid]->lock));
  }

  semaphores[sid]->value--;
  _release(&(semaphores[sid]->lock));
  return SEM_SUCCESS;
}

SEM_RET postSemaphore(semid_t sid) {
  if (!isValidSemaphoreID(sid))
    return SEM_INVALID;
  
  _acquire(&semaphoresLock);
  if (!isActiveSemaphoreID(sid)) {
    _release(&semaphoresLock);
    return SEM_INVALID;
  }
  _acquire(&(semaphores[sid]->lock));
  _release(&semaphoresLock);

  semaphores[sid]->value++;
  queue_value_t wakeProcess;
  char success = peek(semaphores[sid]->waitingQueue, &wakeProcess);
  if (success) {
    dequeue(semaphores[sid]->waitingQueue);
    block(wakeProcess);
  }

  _release(&(semaphores[sid]->lock));
  return SEM_SUCCESS;
}

SEM_RET closeSemaphore(semid_t sid) {
  if (!isValidSemaphoreID(sid))
    return SEM_INVALID;
  
  _acquire(&semaphoresLock);
  if (!isActiveSemaphoreID(sid)) {
    _release(&semaphoresLock);
    return SEM_INVALID;
  }
  _acquire(&(semaphores[sid]->lock));
  if (getCurrentPID() != semaphores[sid]->creatorProcess) {
    _release(&(semaphores[sid]->lock));
    _release(&semaphoresLock);
    return SEM_INVALID;
  }

  // Only destroy if there are no processes waiting?
  // Consider listening queue?
  free(semaphores[sid]);

  _release(&semaphoresLock);
  return SEM_SUCCESS;
}
