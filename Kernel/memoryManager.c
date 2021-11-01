#ifndef BUDDY
/*
 * FreeRTOS Kernel V10.4.3 LTS Patch 1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright (C) 2019-2020 Cypress Semiconductor Corporation, or a subsidiary of
 * Cypress Semiconductor Corporation.  All Rights Reserved.
 *
 * Added support of memory allocation scheme selection from the FreeRTOSConfig.h
 * file using configHEAP_ALLOCATION_SCHEME macro.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 * http://www.cypress.com
 *
 */

/*
 * A sample implementation of alloc() and free() that combines
 * (coalescences) adjacent memory blocks as they are freed, and in so doing
 * limits memory fragmentation.
 *
 * See heap_1.c, heap_2.c and heap_3.c for alternative implementations, and the
 * memory management pages of https://www.FreeRTOS.org for more information.
 */
#include <stdlib.h>
#include <stdint.h>
#include "memoryManager.h"
#include <naiveConsole.h>

/* Known available space for the heap. */
uint8_t * heapStart = (uint8_t *) HEAP_START;

/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE    ( ( size_t ) ( heapStructSize << 1 ) )

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE         ( ( size_t ) 8 )

/* Configure byte alignment. */
#define BYTE_ALIGNMENT  8
#define BYTE_ALIGNMENT_MASK 0x0007

/* Define the linked list structure.  This is used to link free blocks in order
 * of their memory address. */
typedef struct A_BLOCK_LINK
{
    struct A_BLOCK_LINK * pNextFreeBlock; /*<< The next free block in the list. */
    size_t blockSize;                     /*<< The size of the free block. */
} BlockLink_t;

/*-----------------------------------------------------------*/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void insertBlockIntoFreeList( BlockLink_t * pBlockToInsert );

/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
 * block must by correctly byte aligned. */
static const size_t heapStructSize = ( sizeof( BlockLink_t ) + ( ( size_t ) ( BYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) BYTE_ALIGNMENT_MASK );

/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t startBlock, * pEndBlock = NULL;

/* Keeps track of the number of calls to allocate and free memory as well as the
 * number of free bytes remaining, but says nothing about fragmentation. */
static size_t freeBytesRemaining = 0U;
static size_t minimumEverFreeBytesRemaining = 0U;
static size_t numberOfSuccessfulAllocations = 0;
static size_t numberOfSuccessfulFrees = 0;

/* Gets set to the top bit of an size_t type.  When this bit in the blockSize
 * member of an BlockLink_t structure is set then the block belongs to the
 * application.  When the bit is free the block is still part of the free heap
 * space. */
static size_t blockAllocatedBit = 0;

/*-----------------------------------------------------------*/

void * alloc( size_t size )
{
    BlockLink_t * pBlock, * pPreviousBlock, * pNewBlockLink;
    void * pReturn = NULL;

    {

        /* Check the requested block size is not so large that the top bit is
         * set.  The top bit of the block size member of the BlockLink_t structure
         * is used to determine who owns the block - the application or the
         * kernel, so it must be free. */
        if( ( size & blockAllocatedBit ) == 0 )
        {
            /* The wanted size must be increased so it can contain a BlockLink_t
             * structure in addition to the requested amount of bytes. */
            if( ( size > 0 ) && 
                ( ( size + heapStructSize ) >  size ) ) /* Overflow check */
            {
                size += heapStructSize;

                /* Ensure that blocks are always aligned. */
                if( ( size & BYTE_ALIGNMENT_MASK ) != 0x00 )
                {
                    /* Byte alignment required. Check for overflow. */
                    if( ( size + ( BYTE_ALIGNMENT - ( size & BYTE_ALIGNMENT_MASK ) ) ) 
                            > size )
                    {
                        size += ( BYTE_ALIGNMENT - ( size & BYTE_ALIGNMENT_MASK ) );
                    }
                    else
                    {
                        size = 0;
                    }  
                }
            } 
            else 
            {
                size = 0;
            }

            if( ( size > 0 ) && ( size <= freeBytesRemaining ) )
            {
                /* Traverse the list from the start	(lowest address) block until
                 * one of adequate size is found. */
                pPreviousBlock = &startBlock;
                pBlock = startBlock.pNextFreeBlock;

                while( ( pBlock->blockSize < size ) && ( pBlock->pNextFreeBlock != NULL ) )
                {
                    pPreviousBlock = pBlock;
                    pBlock = pBlock->pNextFreeBlock;
                }
                
                /* If the end marker was reached then a block of adequate size
                 * was not found. */
                if( pBlock != pEndBlock )
                {
                    /* Return the memory space pointed to - jumping over the
                     * BlockLink_t structure at its start. */
                    pReturn = ( void * ) ( ( ( uint8_t * ) pPreviousBlock->pNextFreeBlock ) + heapStructSize );

                    /* This block is being returned for use so must be taken out
                     * of the list of free blocks. */
                    pPreviousBlock->pNextFreeBlock = pBlock->pNextFreeBlock;
                    
                    /* If the block is larger than required it can be split into
                     * two. */
                    if( ( pBlock->blockSize - size ) > heapMINIMUM_BLOCK_SIZE )
                    {
                        /* This block is to be split into two.  Create a new
                         * block following the number of bytes requested. The void
                         * cast is used to prevent byte alignment warnings from the
                         * compiler. */
                        pNewBlockLink = ( void * ) ( ( ( uint8_t * ) pBlock ) + size );

                        /* Calculate the sizes of two blocks split from the
                         * single block. */
                        pNewBlockLink->blockSize = pBlock->blockSize - size;
                        pBlock->blockSize = size;

                        /* Insert the new block into the list of free blocks. */
                        insertBlockIntoFreeList( pNewBlockLink );
                    }

                    freeBytesRemaining -= pBlock->blockSize;

                    if( freeBytesRemaining < minimumEverFreeBytesRemaining )
                    {
                        minimumEverFreeBytesRemaining = freeBytesRemaining;
                    }

                    /* The block is being returned - it is allocated and owned
                     * by the application and has no "next" block. */
                    pBlock->blockSize |= blockAllocatedBit;
                    pBlock->pNextFreeBlock = NULL;
                    numberOfSuccessfulAllocations++;
                }
            }
        }
    }
    return pReturn;
}
/*-----------------------------------------------------------*/

void free( void * address )
{
    uint8_t * puc = ( uint8_t * ) address;
    BlockLink_t * pLink;

    if( address != NULL )
    {
        /* The memory being freed will have an BlockLink_t structure immediately
         * before it. */
        puc -= heapStructSize;

        /* This casting is to keep the compiler from issuing warnings. */
        pLink = ( void * ) puc;

        if( ( pLink->blockSize & blockAllocatedBit ) != 0 )
        {
            if( pLink->pNextFreeBlock == NULL )
            {
                /* The block is being returned to the heap - it is no longer
                 * allocated. */
                pLink->blockSize &= ~blockAllocatedBit;

                {
                    /* Add this block to the list of free blocks. */
                    freeBytesRemaining += pLink->blockSize;
                    insertBlockIntoFreeList( ( ( BlockLink_t * ) pLink ) );
                    numberOfSuccessfulFrees++;
                }
            }
        }
    }
}
/*-----------------------------------------------------------*/

void heapInit(void)
{
    BlockLink_t * pFirstFreeBlock;
    uint8_t * pucAlignedHeap;
    size_t uAddress;
    size_t totalHeapSize = TOTAL_HEAP_SIZE;

    /* Ensure the heap starts on a correctly aligned boundary. */
    uAddress = ( size_t ) heapStart;

    if( ( uAddress & BYTE_ALIGNMENT_MASK ) != 0 )
    {
        uAddress += ( BYTE_ALIGNMENT - 1 );
        uAddress &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
        totalHeapSize -= uAddress - ( size_t ) heapStart;
    }

    pucAlignedHeap = ( uint8_t * ) uAddress;

    /* startBlock is used to hold a pointer to the first item in the list of free
     * blocks.  The void cast is used to prevent compiler warnings. */
    startBlock.pNextFreeBlock = ( void * ) pucAlignedHeap;
    startBlock.blockSize = ( size_t ) 0;

    /* pEndBlock is used to mark the end of the list of free blocks and is inserted
     * at the end of the heap space. */
    uAddress = ( ( size_t ) pucAlignedHeap ) + totalHeapSize;
    uAddress -= heapStructSize;
    uAddress &= ~( ( size_t ) BYTE_ALIGNMENT_MASK );
    pEndBlock = ( void * ) uAddress;
    pEndBlock->blockSize = 0;
    pEndBlock->pNextFreeBlock = NULL;

    /* To start with there is a single free block that is sized to take up the
     * entire heap space, minus the space taken by pEndBlock. */
    pFirstFreeBlock = ( void * ) pucAlignedHeap;
    pFirstFreeBlock->blockSize = uAddress - ( size_t ) pFirstFreeBlock;
    pFirstFreeBlock->pNextFreeBlock = pEndBlock;

    /* Only one block exists - and it covers the entire usable heap space. */
    minimumEverFreeBytesRemaining = pFirstFreeBlock->blockSize;
    freeBytesRemaining = pFirstFreeBlock->blockSize;

    /* Work out the position of the top bit in a size_t variable. */
    blockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1 );
}
/*-----------------------------------------------------------*/

static void insertBlockIntoFreeList( BlockLink_t * pBlockToInsert )
{
    BlockLink_t * pIterator;
    uint8_t * puc;

    /* Iterate through the list until a block is found that has a higher address
     * than the block being inserted. */
    for( pIterator = &startBlock; pIterator->pNextFreeBlock < pBlockToInsert; pIterator = pIterator->pNextFreeBlock )
    {
        /* Nothing to do here, just iterate to the right position. */
    }
    
    /* Do the block being inserted, and the block it is being inserted after
     * make a contiguous block of memory? */
    puc = ( uint8_t * ) pIterator;

    if( ( puc + pIterator->blockSize ) == ( uint8_t * ) pBlockToInsert )
    {
        pIterator->blockSize += pBlockToInsert->blockSize;
        pBlockToInsert = pIterator;
    }

    /* Do the block being inserted, and the block it is being inserted before
     * make a contiguous block of memory? */
    puc = ( uint8_t * ) pBlockToInsert;

    if( ( puc + pBlockToInsert->blockSize ) == ( uint8_t * ) pIterator->pNextFreeBlock )
    {
        if( pIterator->pNextFreeBlock != pEndBlock )
        {
            /* Form one big block from the two blocks. */
            pBlockToInsert->blockSize += pIterator->pNextFreeBlock->blockSize;
            pBlockToInsert->pNextFreeBlock = pIterator->pNextFreeBlock->pNextFreeBlock;
        }
        else
        {
            pBlockToInsert->pNextFreeBlock = pEndBlock;
        }
    }
    else
    {
        pBlockToInsert->pNextFreeBlock = pIterator->pNextFreeBlock;
    }

    /* If the block being inserted plugged a gab, so was merged with the block
     * before and the block after, then it's pNextFreeBlock pointer will have
     * already been set, and should not be set here as that would make it point
     * to itself. */
    if( pIterator != pBlockToInsert )
    {
        pIterator->pNextFreeBlock = pBlockToInsert;
    }
}

void memoryDump() { 
    ncPrint("----- Heap4 Memory Dump -----");
	ncNewline();
	ncPrint("Total Heap Size: ");
	ncPrintHex(TOTAL_HEAP_SIZE);
    ncPrintChar('h');
	ncNewline();
    ncPrint("Total Free Memory: ");
	ncPrintHex(freeBytesRemaining);
	ncPrint("h Bytes");
	ncNewline();
    ncPrint("-----------------------------");
    ncNewline();
}

#endif