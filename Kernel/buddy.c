#include "buddy.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LEVELS 12 /* 2^12 = 4KB. */
#define BLOCKS_PER_LEVEL(level) (1<<(level))
#define SIZE_OF_BLOCKS_AT_LEVEL(level,total_size) ((total_size) / (1<<(level)))
#define INDEX_OF_POINTER_IN_LEVEL(pointer,level,memory_start,total_size) (((pointer)-(memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(level,total_size)))

static uint8_t memStart[TOTAL_HEAP_SIZE];

typedef struct list_t {
	struct list_t *prev, *next;
	uint8_t level;
} list_t;

list_t free_lists[MAX_LEVELS] = {NULL};

static void list_init(list_t * list);
static int list_is_empty(list_t * list);
static list_t * list_pop(list_t * list);
static void list_push(list_t *list, list_t *entry);
static void list_remove(list_t *entry);
static int getLevel(size_t size);
static list_t * list_find(uint64_t addr, uint8_t level);


int buddyInit() {
	 list_t * link;
	 for (int i = 0; i < MAX_LEVELS; i++) {
			list_init(&free_lists[i]);
	 }
	 link = (list_t*) memStart;
	 list_init(link);
	 list_push(&free_lists[0], link);
	 return 1;
}

void * alloc(size_t desiredSize) {
	uint8_t level = getLevel(desiredSize + sizeof(list_t));
	void * memory;
	list_t * left;
	list_t * right;
	list_t * link;

	uint8_t actualLevel = level;
	while (list_is_empty(&free_lists[actualLevel]) || actualLevel == -1) {
		actualLevel--;
	}

	if (actualLevel == -1) // no memory 
		return NULL;

	list_t * node;
	for (node = list_pop(&free_lists[actualLevel]); actualLevel < level; actualLevel++) {
		uint64_t size = SIZE_OF_BLOCKS_AT_LEVEL(actualLevel + 1, TOTAL_HEAP_SIZE);
		left = node;
		right = node + size;
		list_init(left);
		list_init(right);  
		list_push(&free_lists[actualLevel + 1], left);
		list_push(&free_lists[actualLevel + 1], right); 
	}
	link = list_pop(&free_lists[level]);
	link->level = level;
	memory = (uint64_t) link;
	memory += sizeof(list_t);
	return (void *) memory;
}

/*[                           ]
  [(L)*         |             ]*/

void free(void * memory) {
	if (memory == NULL)
		return;
	memory -= sizeof(list_t);
	list_t * link = (list_t *)memory, * buddy_link;
	uint8_t level = link->level;
	char stop = 0;
	do {
		link = (list_t *)memory;
		size_t size = SIZE_OF_BLOCKS_AT_LEVEL(level, TOTAL_HEAP_SIZE);
		uint8_t index = INDEX_OF_POINTER_IN_LEVEL((uint64_t)memory, level, (uint64_t)memStart, TOTAL_HEAP_SIZE);
		uint64_t buddy;
		
		if ((index & 1) == 0) {
			buddy = memory + size;
		} else {
			buddy = memory - size;
		}
		buddy_link = NULL;
		if (!list_is_empty(&free_lists[level]))
			buddy_link = list_find(buddy, level);
		
		list_init(link);
		list_push(&free_lists[level], link);

		if (buddy_link == buddy) {
			list_remove(link);
			list_remove(buddy_link);
			if ((index & 1) == 0)
				memory = link;
			else
				memory = buddy_link;
		} else {
			stop = 1;
		}
		level--;
	} while(!stop);
}

// --------------
static void list_init(list_t * list) {
	list->next = NULL;
	list->prev = NULL;
}

static int list_is_empty(list_t * list) {
	 return list->next == NULL;
}

static list_t * list_pop(list_t * list) {
	 list_t * actual = list;
	 while (actual->next != NULL) {
			actual = actual->next;
	 }
	 list_remove(actual);
	 return actual;
}

static void list_push(list_t *list, list_t *entry) {
	 list_t * actual = list;
	 while (actual->next != NULL) {
			actual = actual->next;
	 }
	 actual->next = entry;
	 entry->next = NULL;
	 entry->prev = actual;
}

static void list_remove(list_t *entry) {
	list_t *prev = entry->prev;
	list_t *next = entry->next;
	if (prev != NULL)
		prev->next = next;
	if (next != NULL)
		next->prev = prev;
}

static list_t * list_find(uint64_t addr, uint8_t level) {
	list_t * aux = &free_lists[level];
	aux = aux->next;
	while (aux != NULL) {
		if (aux == addr) {
			return aux;
		}
		aux = aux->next;
	}
	return NULL;
}
// --------------

static int getLevel(size_t size) {
	int level = 0;
	uint64_t currSize = TOTAL_HEAP_SIZE;
	if (size > currSize)
		return -1;
	while (size <= currSize/2) {
		level++; 
		currSize /= 2;
	}
	return level;
}