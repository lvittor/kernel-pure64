#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>

typedef uint64_t queue_value_t;
typedef struct queueCDT * queueADT;

queueADT newQueue(void);
char isEmpty(queueADT q);
char enqueue(queueADT q, queue_value_t v);
queue_value_t peek(queueADT q);
void dequeue(queueADT q);
void freeQueue(queueADT q);

#endif