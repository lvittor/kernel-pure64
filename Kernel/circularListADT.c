#include <circularListADT.h>
#include <mmgr.h>

typedef struct node *TList;

typedef struct node {
    elemType elem;
    TList next;
} TNode;

typedef struct circularListCDT {
    TList previous, current;
    uint8_t size;  // TODO: check size's size.
} circularListCDT;

circularListADT newCircularList(void) {
    circularListADT ans = alloc(sizeof(circularListCDT));
    if (ans == NULL) {
        return ans;
    }
    ans->size = 0;
    ans->previous = ans->current = NULL;
    return ans;
}

int addToList(circularListADT list, elemType elem) {
    if (list == NULL) {
        return -1;  // TODO: Define errors
    }

    TList newNode = alloc(sizeof(TNode));
    if (newNode == NULL) {
        return -1;
    }
    newNode->elem = elem;
    
    if (list->current == NULL) {
        list->current = list->previous = newNode;
        list->current->next = list->previous->next = newNode;
    } else {
        newNode->next = list->current;
        list->previous->next = newNode;
        list->previous = newNode;
    }

    return ++list->size;
}


uint8_t listSize(circularListADT list) {
    return list->size;
}

void moveToNext(circularListADT list) {
    if (list == NULL || list->current == NULL) {
        return;
    }

    list->previous = list->current;
    list->current = list->current->next;
    return;
}

elemType getCurrent(circularListADT list) {
    if (list == NULL || list->current == NULL) {
        return NULL;
    }

    return list->current->elem;
}

int deleteCurrent(circularListADT list) {
    if (list == NULL || list->current == NULL) {
        return -1;
    }

    if (list->current == list->previous) {
        free(list->current);
        list->current = list->previous = NULL;
        return 2;
    }

    TList aux = list->current;
    list->previous->next = list->current = list->current->next;
    free(aux);
    return 1;
}


