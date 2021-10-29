#include <queue.h>
#include <mmgr.h>
#include <naiveConsole.h>

typedef struct node {
    elemType elem;
    struct node *next;
} Node;

typedef struct QueueCDT {
    uint8_t size;
    Node *first, *last;
} QueueCDT;

Queue newQueue(void) {
    Queue ans = alloc(sizeof(QueueCDT));
    if (ans == NULL) {
        return ans;
    }
    ans->size = 0;
    ans->first = ans->last = NULL;
    return ans;
}

int push(Queue queue, elemType elem) {
    if (queue == NULL) {
        return -1;  // TODO: Define errors
    }

    Node *newNode = alloc(sizeof(Node));
    if (newNode == NULL) {
        return -1;
    }
    newNode->elem = elem;
    newNode->next = NULL;
    
    if (queue->first == NULL) {
        queue->first = queue->last = newNode;
    } else {
        queue->last->next = newNode;
        queue->last = newNode;
    }

    return ++queue->size;
}

elemType pop(Queue queue) {
    if (queue == NULL || queue->first == NULL) {
        return -1;
    }

    elemType elem = queue->first->elem;

    if (queue->first == queue->last) {
        queue->last = NULL;
    }

    Node *aux = queue->first;
    queue->first = queue->first->next;
    free(aux);
    queue->size--;
    return elem;
}

uint8_t queueSize(Queue queue){
    return queue == NULL ? 0 : queue->size;
}

uint8_t isEmpty(Queue queue){
    return queue == NULL? 1 : queue->size == 0;
}

void printQueue(Queue queue) {
    ncPrint("{");
    if(queue == NULL){
        ncPrint("}");
        return;
    }

    Node *aux = queue->first;
    while(aux != NULL) {
        ncPrintDec(aux->elem);
        ncPrint(", ");
        aux = aux->next;
    }
    ncPrint("}");
}
