#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stdint.h>
#include <stddef.h>

void keyboard_handler(void);
long copy_from_buffer(char * buf, size_t count);

#endif
