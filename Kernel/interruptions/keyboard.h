#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define EOF -1

void keyboard_handler(void);
int64_t getChar(void);
int initKeyboard(void);

#endif
