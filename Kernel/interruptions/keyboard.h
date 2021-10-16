#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include <stddef.h>
#include <stdint.h>


void keyboard_handler(void);
long copy_from_buffer(char *buf, size_t count);

#endif
