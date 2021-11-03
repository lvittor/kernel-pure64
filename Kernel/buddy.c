#ifdef BUDDY

#include <mm.h>
#include <naiveConsole.h>

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LEVELS 21
#define BLOCKS_PER_LEVEL(level) (1 << (level))
#define SIZE_OF_BLOCKS_AT_LEVEL(level, total_size)                             \
	((total_size) / (1 << (level)))
#define INDEX_OF_POINTER_IN_LEVEL(pointer, level, memory_start, total_size)    \
	(((pointer) - (memory_start)) /                                        \
	 (SIZE_OF_BLOCKS_AT_LEVEL(level, total_size)))

uint8_t *mem_start = (uint8_t *)HEAP_START;

typedef struct _list_t * list_t;

typedef struct _list_t {
	list_t prev, next;
	uint8_t level;
} _list_t;

_list_t free_lists[MAX_LEVELS];

static void list_init(list_t list);
static int list_is_empty(list_t list);
static list_t list_pop(list_t list);
static void list_push(list_t list, list_t entry);
static void list_remove(list_t entry);
static int get_level(size_t size);
static list_t list_find(uint64_t addr, uint8_t level);
static uint64_t list_level_print(uint8_t level);

void heap_init()
{
	list_t link;
	for (int i = 0; i < MAX_LEVELS; i++) {
		list_init(&free_lists[i]);
	}
	link = (list_t)mem_start;
	list_init(link);
	list_push(&free_lists[0], link);
}

void *alloc(size_t size)
{
	int level = get_level(size + sizeof(_list_t));
	if (level == -1)
		return NULL;
	void *memory;
	void *left;
	void *right;
	list_t link;

	int actualLevel = level;
	while (actualLevel >= 0) {
		if (!list_is_empty(&free_lists[actualLevel]))
			break;
		actualLevel--;
	}

	if (actualLevel == -1) // no memory
		return NULL;

	void *node;
	for (; actualLevel < level; actualLevel++) {
		node = (void *)list_pop(&free_lists[actualLevel]);
		uint64_t blockSize = SIZE_OF_BLOCKS_AT_LEVEL(actualLevel + 1,
							     TOTAL_HEAP_SIZE);
		left = node;
		right = node + blockSize;
		list_init(left);
		list_init(right);
		list_push(&free_lists[actualLevel + 1], left);
		list_push(&free_lists[actualLevel + 1], right);
	}
	link = list_pop(&free_lists[level]);
	link->level = level;
	memory = link;
	memory += sizeof(_list_t);
	return (void *)memory;
}

void free(void *address)
{
	if (address == NULL)
		return;
	address -= sizeof(_list_t);
	list_t link = (list_t)address, buddy_link;
	uint8_t level = link->level;
	char stop = 0;
	do {
		link = (list_t)address;
		size_t size = SIZE_OF_BLOCKS_AT_LEVEL(level, TOTAL_HEAP_SIZE);
		uint8_t idx =
			INDEX_OF_POINTER_IN_LEVEL((uint64_t)address, level,
						  (uint64_t)mem_start,
						  TOTAL_HEAP_SIZE);
		uint64_t buddy;

		if ((idx & 1) == 0) {
			buddy = (uint64_t)(address + size);
		} else {
			buddy = (uint64_t)(address - size);
		}

		buddy_link = NULL;
		if (!list_is_empty(&free_lists[level]))
			buddy_link = list_find(buddy, level);

		list_init(link);
		list_push(&free_lists[level], link);

		if ((uint64_t)buddy_link == buddy) {
			list_remove(link);
			list_remove(buddy_link);
			if ((idx & 1) == 0)
				address = link;
			else
				address = buddy_link;
		} else {
			stop = 1;
		}
		level--;
	} while (!stop);
}

static void list_init(list_t list)
{
	list->next = NULL;
	list->prev = NULL;
}

static int list_is_empty(list_t list)
{
	return list->next == NULL;
}

static list_t list_pop(list_t list)
{
	list_t actual = list;
	while (actual->next != NULL) {
		actual = actual->next;
	}
	list_remove(actual);
	return actual;
}

static void list_push(list_t list, list_t entry)
{
	list_t actual = list;
	while (actual->next != NULL) {
		actual = actual->next;
	}
	actual->next = entry;
	entry->next = NULL;
	entry->prev = actual;
}

static void list_remove(list_t entry)
{
	list_t prev = entry->prev;
	list_t next = entry->next;
	if (prev != NULL)
		prev->next = next;
	if (next != NULL)
		next->prev = prev;
}

static list_t list_find(uint64_t addr, uint8_t level)
{
	list_t aux = &free_lists[level];
	aux = aux->next;
	while (aux != NULL) {
		if ((uint64_t)aux == addr) {
			return aux;
		}
		aux = aux->next;
	}
	return NULL;
}

static uint64_t list_level_print(uint8_t level)
{
	uint64_t totalMem = 0;
	list_t it = &free_lists[level];
	uint8_t actual = 0;
	size_t size = SIZE_OF_BLOCKS_AT_LEVEL(level, TOTAL_HEAP_SIZE);
	it = it->next;
	while (it != NULL) {
		actual++;
		it = it->next;
	}
	totalMem = size * actual;
	ncPrintDec(actual);
	ncPrint(" blocks of ");
	ncPrintHex(size);
	ncPrint("h Bytes");
	ncNewline();
	return totalMem;
}

static int get_level(size_t size)
{
	int level = 0;
	uint64_t currSize = TOTAL_HEAP_SIZE;
	if (size > currSize)
		return -1;
	while (size <= currSize / 2 && level < MAX_LEVELS - 1) {
		level++;
		currSize /= 2;
	}
	return level;
}

void dump_mem()
{
	size_t totalMem = 0;
	ncPrint("----- Dummy Memory Dump -----");
	ncNewline();
	ncPrint("Total Heap Size: ");
	ncPrintHex(TOTAL_HEAP_SIZE);
	ncPrintChar('h');
	ncNewline();
	for (uint8_t i = 0; i < MAX_LEVELS; i++) {
		if (!list_is_empty(&free_lists[i])) {
			ncPrint("Level ");
			ncPrintDec(i);
			ncPrint(": ");
			totalMem += list_level_print(i);
		}
	}
	ncPrint("Total Free Memory: ");
	ncPrintHex(totalMem);
	ncPrint("h Bytes");
	ncNewline();
	ncPrint("-----------------------------");
	ncNewline();
}

#endif