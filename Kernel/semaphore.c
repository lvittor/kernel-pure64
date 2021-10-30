#include <semaphore.h>
#include <stddef.h>
#include <memoryManager.h>

static lock_t semaphoresLock = 0;
struct semaphoreCDT * semaphores[MAX_SEMAPHORES] = {NULL};

typedef struct semaphoreCDT {
  lock_t lock;
  semvalue_t value;
  // queueADT waiting;
} semaphoreCDT;

static char isValidSemaphoreID(semid_t sid) {
  return sid >= 0 && sid < MAX_SEMAPHORES;
}

static char isActiveSemaphoreID(semid_t sid) {
  return isValidSemaphoreID(sid) && semaphores[sid] != NULL;
}

enum SEM_RET createSemaphore(semid_t sid, semvalue_t value) {
  if (!isValidSemaphoreID(sid))
    return SEM_INVALID;
  
  _acquire(&semaphoresLock);
  
  if (isActiveSemaphoreID(id)) {
    _release(&semaphoresLock);
    return SEM_EXISTS;
  }
  
  semaphores[sid] = alloc(sizeof(semaphoreCDT));
  if (semaphores[sid] == NULL) {
    _release(&semaphoresLock);
    return SEM_ENOMEM;
  }

  semaphores[sid]->lock = 0;
  semaphores[sid]->value = value;
  _release(&semaphoresLock);
  return SEM_SUCCESS;
}

enum SEM_RET waitSemaphore(semid_t sid) {
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
    _release(&(semaphores[sid]->lock))
    // sleep(s.chan)
    // TODO: Add to queue and go to sleep(Blocked?) 
    // se despierta solo cuando alguien hace post
    _acquire(&(semaphores[sid]->lock))
  }

  semaphores[sid]->value--;
  _release(&(semaphores[sid]->lock))
  return SEM_SUCCESS;
}

enum SEM_RET postSemaphore(semid_t sid) {
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
  // wakeup(s.chan)

  _release(&(semaphores[sid]->lock));
  return SEM_SUCCESS;
}

enum SEM_RET closeSemaphore(semid_t sid) {
  if (!isValidSemaphoreID(sid))
    return SEM_INVALID;
  
  _acquire(&semaphoresLock);
  if (!isActiveSemaphoreID(sid)) {
    _release(&semaphoresLock);
    return SEM_INVALID;
  }
  _acquire(&(semaphores[sid]->lock));

  // Only destroy if there are no processes waiting
  // Consider listening queue
  free(semaphores[sid]);

  _release(&semaphoresLock);
  return SEM_SUCCESS;
}
