#include "mmgr.h"
#include <stdint.h>
#define TOTAL_HEAP_SIZE 8192
#define MINIMUM_ALLOCABLE_SIZE 8

static char to_alloc[TOTAL_HEAP_SIZE];

typedef struct BlockLink {
    struct BlockLink *nextFreeBlock;
    size_t blockSize;
} BlockLink;

void addToList(BlockLink *newBlock);

static const size_t blockLinkSize = sizeof(BlockLink); // TODO: check alignment, if necessary
static const size_t minimumAllocableSize = MINIMUM_ALLOCABLE_SIZE;
static size_t freeRemainingBytes = 0;
static size_t totalHeapSize = (size_t) TOTAL_HEAP_SIZE;
static size_t startingHeapAddress = (size_t) to_alloc;
static BlockLink startBlock, *endBlock = NULL;


int initMgr(){
    startBlock.blockSize = 0;
    BlockLink *firstFreeBlock = (void *) startingHeapAddress;
    endBlock = (void *) (startingHeapAddress + totalHeapSize - blockLinkSize);
    endBlock->blockSize = 0;
    endBlock->nextFreeBlock = NULL;
    firstFreeBlock->blockSize = totalHeapSize - 2 * blockLinkSize;
    firstFreeBlock->nextFreeBlock = endBlock;
    startBlock.nextFreeBlock = firstFreeBlock;
    return 1;
}

void *alloc(size_t bytes){
    BlockLink *iterator = &startBlock, *newBlock;
    void *ptr = NULL;
    if(bytes > 0){
        bytes = bytes < minimumAllocableSize? minimumAllocableSize : bytes;
        while(iterator->blockSize < bytes && iterator->nextFreeBlock != NULL)
            iterator = iterator->nextFreeBlock;
        if(iterator != endBlock){
            ptr = (void *) ((uint8_t *) iterator + blockLinkSize);
            newBlock = (void *) ((uint8_t *) ptr + bytes);
            newBlock->blockSize = iterator->blockSize - bytes;
            addToList(newBlock);
        }
    }
    return ptr;
}

void free(void *ptr){
    if(ptr == NULL){
        return;
    }
    BlockLink *freedBlock = (void *) ((uint8_t *) ptr - blockLinkSize);
    addToList(freedBlock);
}

// TODO: coalesce adjacent blocks.
void addToList(BlockLink *newBlock){
    BlockLink *iterator = &startBlock;

    while(iterator->nextFreeBlock < newBlock){
        iterator = iterator->nextFreeBlock;
    }

    newBlock->nextFreeBlock = iterator->nextFreeBlock;
    iterator->nextFreeBlock = newBlock;
}
    