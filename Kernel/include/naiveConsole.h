#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

#define WHITE_ON_BLACK	0x07
#define RED_ON_BLACK	0x04

void ncPrint(const char * string);
void ncPrintAtt(const char * string, char frontColor, char backColor);
void ncPrintChar(char character);
void ncPrintCharAtt(char character, char attribute);
void ncNewline();
void ncPrintBCD(uint64_t value);
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

#endif