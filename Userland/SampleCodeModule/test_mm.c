#include <stdio.h>
#include <stdlib.h>
#include <test_util.h>
#include <string.h>
#include <lib.h>

#define MAX_BLOCKS 128 // TODO: Check
#define MAX_MEMORY 0x6400000 //Should be around 80% of memory managed by the MM

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm(void){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;

  print_f(STDOUT, "Comenzando la prueba...\n");

  while (1){
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = _alloc(mm_rqs[rq].size);
      if (mm_rqs[rq].address == NULL)
        break;
      total += mm_rqs[rq].size;
      rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        memwrite(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size))
          print_f(STDERR, "F");

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        _free(mm_rqs[i].address);
    
    print_f(STDOUT, ".");
  }

  _exit();
}