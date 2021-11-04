#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdint.h>

typedef uint64_t queue_value_t;
typedef struct _queue_t *queue_t;

queue_t queue_init(void);
char is_empty(queue_t q);
char enqueue(queue_t q, queue_value_t v);
char peek(queue_t q, queue_value_t *v);
void dequeue(queue_t q);
void free_queue(queue_t q);
void dump_queue(queue_t q);

#endif