#include <semaphore.h>
#include <stddef.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <queue.h>
#include <lib.h>
#include <naiveConsole.h>

static lock_t semaphoresLock = 0;
struct semaphoreCDT * semaphores[MAX_SEMAPHORES] = {NULL};

typedef struct semaphoreCDT {
  char * name;
  lock_t lock;
  uint8_t creatorProcess;
  semvalue_t value;
  queueADT waitingQueue;
} semaphoreCDT;

static uint8_t getIndexFromSID(semid_t sid) {
  for (uint8_t i = 0; i < MAX_SEMAPHORES; i++) {
    if (semaphores[i] != NULL && strcmp(semaphores[i]->name, sid) == 0)
      return i;
  }
  return MAX_SEMAPHORES;
}

static uint8_t getFreeIndex() {
  for (uint8_t i = 0; i < MAX_SEMAPHORES; i++) {
    if (semaphores[i] == NULL)
      return i;
  }
  return MAX_SEMAPHORES;
}

SEM_RET openSemaphore(semid_t sid, semvalue_t value) {
  _acquire(&semaphoresLock);
  uint8_t semIndex = getIndexFromSID(sid);
  if (semIndex != MAX_SEMAPHORES) {
    _release(&semaphoresLock);
    return SEM_EXISTS;
  }

  semIndex = getFreeIndex();
  semaphores[semIndex] = alloc(sizeof(semaphoreCDT));
  if (semaphores[semIndex] == NULL) {
    _release(&semaphoresLock);
    return SEM_ENOMEM;
  }

  semaphores[semIndex]->waitingQueue = newQueue();
  if (semaphores[semIndex]->waitingQueue == NULL) {
    free(semaphores[semIndex]);
    _release(&semaphoresLock);
    return SEM_ENOMEM;
  }

  semaphores[semIndex]->lock = 0;
  semaphores[semIndex]->value = value;
  semaphores[semIndex]->creatorProcess = getCurrentPID();
  _release(&semaphoresLock);
  return SEM_SUCCESS;
}

SEM_RET waitSemaphore(semid_t sid) {
  _acquire(&semaphoresLock);
  uint8_t semIndex = getIndexFromSID(sid);
  if (semIndex == MAX_SEMAPHORES) {
    _release(&semaphoresLock);
    return SEM_INVALID;
  }
  _acquire(&(semaphores[semIndex]->lock));
  _release(&semaphoresLock);

  while (semaphores[semIndex]->value <= 0){
    setPIDState(getCurrentPID(), BLOCKED);
    enqueue(semaphores[semIndex]->waitingQueue, getCurrentPID());
    _release(&(semaphores[semIndex]->lock));
    yieldProcess();
    _acquire(&(semaphores[semIndex]->lock));
  }

  semaphores[semIndex]->value--;
  _release(&(semaphores[semIndex]->lock));
  return SEM_SUCCESS;
}

SEM_RET postSemaphore(semid_t sid) {
  _acquire(&semaphoresLock);
  uint8_t semIndex = getIndexFromSID(sid);
  if (semIndex == MAX_SEMAPHORES) {
    _release(&semaphoresLock);
    return SEM_INVALID;
  }
  _acquire(&(semaphores[semIndex]->lock));
  _release(&semaphoresLock);

  semaphores[semIndex]->value++;
  queue_value_t wakeProcess;
  char success = peek(semaphores[semIndex]->waitingQueue, &wakeProcess);
  if (success) {
    dequeue(semaphores[semIndex]->waitingQueue);
    block(wakeProcess);
  }

  _release(&(semaphores[semIndex]->lock));
  return SEM_SUCCESS;
}

SEM_RET closeSemaphore(semid_t sid) {
  _acquire(&semaphoresLock);
  uint8_t semIndex = getIndexFromSID(sid);
  if (semIndex == MAX_SEMAPHORES) {
    _release(&semaphoresLock);
    return SEM_INVALID;
  }
  _acquire(&(semaphores[semIndex]->lock));
  if (getCurrentPID() != semaphores[semIndex]->creatorProcess) {
    _release(&(semaphores[semIndex]->lock));
    _release(&semaphoresLock);
    return SEM_INVALID;
  }

  // Only destroy if there are no processes waiting?
  // Consider listening queue?
  free(semaphores[semIndex]);

  _release(&semaphoresLock);
  return SEM_SUCCESS;
}

void printSemaphores(void) {
  ncPrint("Semaphores\n");
  _acquire(&semaphoresLock);
  for (uint8_t i = 0; i < MAX_SEMAPHORES; i++) {
    if (semaphores[i] != NULL) {
    _acquire(&(semaphores[i]->lock));
    ncPrint(semaphores[i]->name);
    ncPrint(": ");
    ncPrintDec(semaphores[i]->value);
    ncPrint(", ");
    printQueue(semaphores[i]->waitingQueue);
    _release(&(semaphores[i]->lock));
    ncNewline();
    }
  }
  ncNewline();
  _release(&semaphoresLock);
}
