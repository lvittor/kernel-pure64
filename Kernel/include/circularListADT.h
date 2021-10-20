#ifndef __CIRCULAR_LIST_ADT_H__
#define __CIRCULAR_LIST_ADT_H__

#include <stdint.h>

typedef struct circularListCDT * circularListADT;
typedef uint64_t elemType;

circularListADT newCircularList(void);

int addToList(circularListADT list, elemType elem);

uint8_t listSize(circularListADT list);

void moveToNext(circularListADT list);

elemType getCurrent(circularListADT list);

int deleteCurrent(circularListADT list);

#endif