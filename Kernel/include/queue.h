#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>
#include <process.h>

typedef struct QueueCDT * Queue;
typedef pid_t elemType;

Queue newQueue(void);

int push(Queue queue, elemType elem);

elemType pop(Queue queue);

uint8_t queueSize(Queue queue);

uint8_t isEmpty(Queue queue);

void printQueue(Queue queue);

#endif