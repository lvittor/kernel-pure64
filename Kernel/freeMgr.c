#include <mmgr.h>
#include <stdint.h>

#define TOTAL_HEAP_SIZE 8192 // 2 ^ 13

#define MINIMUM_ALLOCABLE_SIZE ((size_t)(blockLinkSize << 1))

#define BYTE_ALIGNMENT 8
#define BYTE_ALIGNMENT_MASK 0x0007

static char to_alloc[TOTAL_HEAP_SIZE];

typedef struct BlockLink {
  struct BlockLink *nextFreeBlock;
  size_t blockSize;
} BlockLink;

void addToFreeList(BlockLink *newBlock);

static const size_t blockLinkSize =
    (sizeof(BlockLink) + ((size_t)(BYTE_ALIGNMENT - 1))) &
    ~((size_t)BYTE_ALIGNMENT_MASK);
static size_t freeRemainingBytes = 0U;
static size_t totalHeapSize = 0U;
static BlockLink startBlock, *endBlock = NULL;

/* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t blockAllocatedBit = 0; // TODO: Ask why is this needed.

int initMgr() {
  BlockLink *firstFreeBlock;
  uint8_t *alignedHeap;
  size_t hAddress = (size_t)to_alloc;
  totalHeapSize = (size_t)TOTAL_HEAP_SIZE;

  if ((hAddress & BYTE_ALIGNMENT_MASK) != 0) {
    hAddress += BYTE_ALIGNMENT - 1;
    hAddress &= ~((size_t)BYTE_ALIGNMENT_MASK);
    totalHeapSize -= hAddress - (size_t)to_alloc;
  }

  alignedHeap = (uint8_t *)hAddress;

  startBlock.blockSize = 0;
  startBlock.nextFreeBlock = (void *)alignedHeap;

  hAddress = ((size_t)alignedHeap) + totalHeapSize;
  hAddress -= blockLinkSize;
  hAddress &= ~((size_t)BYTE_ALIGNMENT_MASK); // TODO: Ask why this is needed.

  endBlock = (void *)hAddress;
  endBlock->blockSize = 0;
  endBlock->nextFreeBlock = NULL;

  firstFreeBlock = (void *)alignedHeap;
  firstFreeBlock->blockSize = hAddress - (size_t)firstFreeBlock;
  firstFreeBlock->nextFreeBlock = endBlock;

  freeRemainingBytes = firstFreeBlock->blockSize;

  blockAllocatedBit = ((size_t)1) << ((sizeof(size_t) * BYTE_ALIGNMENT) - 1);
  return 1;
}

void *alloc(size_t size) {
  void *allocPtr = NULL;

  if (size > 0) {
    BlockLink *iterator = &startBlock, *previous;
    size += blockLinkSize;

    if ((size & BYTE_ALIGNMENT_MASK) != 0) {
      size += (BYTE_ALIGNMENT - (size & BYTE_ALIGNMENT_MASK));
    }

    if (size > freeRemainingBytes)
      return NULL;

    while (iterator->blockSize < size && iterator->nextFreeBlock != NULL) {
      previous = iterator;
      iterator = iterator->nextFreeBlock;
    }

    if (iterator != endBlock) {
      /* Return the memory space pointed to - jumping over the
         BlockLink_t structure at its start. */
      allocPtr = (void *)((uint8_t *)iterator + blockLinkSize);

      /* This block is being returned for use so must be taken out
         of the list of free blocks. */
      previous->nextFreeBlock = iterator->nextFreeBlock;

      if (iterator->blockSize - size > MINIMUM_ALLOCABLE_SIZE) {
        BlockLink *newBlock = (void *)((uint8_t *)allocPtr + size);
        newBlock->blockSize = iterator->blockSize - size;
        iterator->blockSize = size;
        addToFreeList(newBlock);
      }

      freeRemainingBytes -= iterator->blockSize;

      /* The block is being returned - it is allocated and owned
         by the application and has no "next" block. */
      iterator->blockSize |= blockAllocatedBit;
      iterator->nextFreeBlock = NULL;
    }
  }
  return allocPtr;
}

void free(void *ptr) {
  if (ptr == NULL) {
    return;
  }
  BlockLink *freedBlock = (void *)(((uint8_t *)ptr) - blockLinkSize);
  if ((freedBlock->blockSize & blockAllocatedBit) != 0 &&
      freedBlock->nextFreeBlock == NULL) {
    /* The block is being returned to the heap - it is no longer
       allocated. */
    freedBlock->blockSize &= ~blockAllocatedBit;
    { // TODO: Ask how we should manage these share memory issues.
      freeRemainingBytes += freedBlock->blockSize;
      // TODO: traceFREE() ????
      addToFreeList(freedBlock);
    }
  }
}

void addToFreeList(BlockLink *newBlock) {
  BlockLink *iterator = &startBlock;
  uint8_t *puc;

  while (iterator->nextFreeBlock < newBlock) {
    iterator = iterator->nextFreeBlock;
  }

  /* Do the block being inserted, and the block it is being inserted after
     make a contiguous block of memory? */

  puc = (uint8_t *)iterator;
  if (puc + iterator->blockSize == (uint8_t *)newBlock) {
    iterator->blockSize += newBlock->blockSize;
    newBlock = iterator;
  }

  /* Do the block being inserted, and the block it is being inserted before
     make a contiguous block of memory? */
  puc = (uint8_t *)newBlock;
  if ((puc + newBlock->blockSize) == (uint8_t *)iterator->nextFreeBlock) {
    if (iterator->nextFreeBlock != endBlock) {
      /* Form one big block from the two blocks. */
      newBlock->blockSize += iterator->nextFreeBlock->blockSize;
      newBlock->nextFreeBlock = iterator->nextFreeBlock->nextFreeBlock;
    } else {
      newBlock->nextFreeBlock = endBlock;
    }
  } else {
    newBlock->nextFreeBlock = iterator->nextFreeBlock;
  }

  /* If the block being inserted plugged a gap, so was merged with the block
          before and the block after, then it's nextFreeBlock pointer will have
          already been set, and should not be set here as that would make it
     point to itself. */
  if (iterator != newBlock) {
    iterator->nextFreeBlock = newBlock;
  }
}
