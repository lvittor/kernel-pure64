#include <queue.h>
#include <mm.h>
#include <naiveConsole.h>

typedef struct _queue_node_t *queue_node_t;

typedef struct _queue_node_t {
	queue_value_t value;
	queue_node_t next;
} _queue_node_t;

typedef struct _queue_t {
	queue_node_t front, rear;
} _queue_t;

queue_t queue_init(void)
{
	queue_t q = alloc(sizeof(_queue_t));
	if (q == NULL)
		return NULL;
	q->front = NULL;
	q->rear = NULL;
	return q;
}

char is_empty(queue_t q)
{
	if (q == NULL)
		return 0;
	return q->front == NULL;
}

char enqueue(queue_t q, queue_value_t v)
{
	if (q == NULL)
		return 0;
	queue_node_t node = alloc(sizeof(_queue_node_t));
	if (node == NULL)
		return 0;
	node->value = v;
	node->next = NULL;
	if (is_empty(q)) {
		q->front = node;
		q->rear = node;
	} else {
		q->rear->next = node;
		q->rear = node;
	}
	return 1;
}

char peek(queue_t q, queue_value_t *v)
{
	if (is_empty(q))
		return 0;
	*v = q->front->value;
	return 1;
}

void dequeue(queue_t q)
{
	if (is_empty(q))
		return;
	queue_node_t gone = q->front;
	q->front = gone->next;
	free(gone);
	if (q->front == NULL)
		q->rear = NULL;
}

void free_queue(queue_t q)
{
	if (q == NULL)
		return;
	while (!is_empty(q))
		dequeue(q);
	free(q);
}

void dump_queue(queue_t q)
{
	if (q == NULL)
		return;
	ncPrintChar('[');
	queue_node_t it = q->front;
	while (it != NULL) {
		ncPrintDec(it->value);
		ncPrint(", ");
	}
	ncPrintChar(']');
}