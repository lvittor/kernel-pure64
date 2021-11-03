#include <stdint.h>

uint32_t GetUint(void);
uint32_t GetUniform(uint32_t max);
void memwrite(void *destiation, int32_t c, uint64_t length);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);

// Tests
void test_mm(void);
