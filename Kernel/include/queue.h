#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>

typedef uint64_t queue_value_t;
typedef struct queueCDT * queueADT;

queueADT newQueue(void);
char isEmpty(queueADT q);
char enqueue(queueADT q, queue_value_t v);
char peek(queueADT q, queue_value_t * v);
void dequeue(queueADT q);
void freeQueue(queueADT q);
void printQueue(queueADT q);

#endif