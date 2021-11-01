#include <queue.h>
#include <memoryManager.h>

typedef struct nodeCDT {
    queue_value_t value;
    struct nodeCDT * next;
} nodeCDT;

typedef struct nodeCDT * nodeADT;

typedef struct queueCDT {
    nodeCDT * front, * rear;
} queueCDT;

queueADT newQueue(void) {
    queueADT q = alloc(sizeof(queueCDT));
    if (q == NULL)
        return NULL;
    q->front = NULL;
    q->rear = NULL;
    return q;    
}

char isEmpty(queueADT q) {
    if (q == NULL)
        return 0;
    return q->front == NULL;
}

char enqueue(queueADT q, queue_value_t v) {
    if (q == NULL)
        return 0;
    nodeADT node = alloc(sizeof(nodeCDT));
    if (node == NULL)
        return 0;
    node->value = v;
    node->next = NULL;
    if (isEmpty(q)) {
        q->front = node;
        q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    return 1;
}

queue_value_t peek(queueADT q) {
    if (isEmpty(q))
        return 0;
    return q->front->value;
}

void dequeue(queueADT q) {
    if (isEmpty(q))
        return;
    nodeADT gone = q->front;
    q->front = gone->next;
    free(gone);
    if (q->front == NULL)
        q->rear = NULL;
}

void freeQueue(queueADT q) {
    if (q == NULL)
        return;
    while(!isEmpty(q))
        dequeue(q);
    free(q);
}