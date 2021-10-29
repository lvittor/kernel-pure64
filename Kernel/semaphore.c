#include <semaphore.h>

#define 

typedef struct semaphoreCDT {
     char lock;
    uint8_t value;
    queueADT waiting;
} semaphoreCDT;


int8_t createSemaphore(uint8_t id) {
    // TODO: lock para manipular la lista de semaphores?
    // Chequear si está libre el id
    // si no, error
    // Si sí, creamos
}

void sem_wait(uint8_t id) {
  _acquire(&(s.lock)) //spinlock (xchg)

  if (s.value > 0){
    s.value--;
  }else{
    _release(&(s.lock))
    // TODO: Add to queue and go to sleep(Blocked?) sleep(s.chan)
    // se despierta solo cuando alguien hace post
    _acquire(&(s.lock))
    s.value--;
  }

  _release(&(s.lock)) //spinlock
}

sem_post(s){
  _acquire(&(s.lock))

  s.value++;
  // TODO: pop first from queue and remove Blocked state: wakeup(s.chan)

  _release(&(s.lock))
}

void closeSemaphore() {

}



