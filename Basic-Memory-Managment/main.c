#include "mmgr.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main() {
  initMgr();
  char *str = alloc(0), memory[1024];
  assert(str == NULL);
  str = alloc(8192);
  assert(str == NULL);
  str = alloc(20);
  uint64_t address = (uint64_t)str;
  strcpy(str, "Hola Mundo!");
  assert(strcmp(str, "Hola Mundo!") == 0);
  free(str);

  str = memory;
  strcpy(str, "Hola Mundo!");
  assert(strcmp(str, "Hola Mundo!") == 0);

  str = alloc(20);
  assert((uint64_t)str == address);
  free(str);

  char *s1 = alloc(40), *s2 = alloc(40);
  strcpy(s1, "lorem ipsum");
  strcpy(s2, s1);
  assert(strcmp(s1, s2) == 0);
  free(s1);
  free(s2);

  s1 = alloc(20);
  assert(s1 != NULL);
  free(s1);
  puts("OK");
  return 0;
}