#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>
#include <video.h>

extern prompt_info prompt;

void ncPrint(const char * string);
void ncPrintAtt(const char * string, Color * frontColor, Color * backColor);
void ncPrintChar(char character);
void ncPrintCharAtt(char character, Color * fontColor, Color * backgroundColor);
void ncPrintBCD(uint64_t value);
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncNewline(void);

#endif