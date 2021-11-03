#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>

#define EOF -1

void keyboard_handler(void);
int get_char(void);
int init_keyboard(void);

#endif
