#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>
#include <sched.h>
#include <sem.h>

#define LEFT_SHIFT 0x2A
#define LEFT_SHIFT_FLAG 0b00000001
#define RIGHT_SHIFT 0x36
#define RIGHT_SHIFT_FLAG 0b00000010
#define LEFT_ALT 0x38
#define ESC 0x01

#define BUFFER_SIZE 128

#define KEYBOARD_SEM_ID "SEM_KEYB"

uint8_t flags = 0;

static int buffer[BUFFER_SIZE];
uint64_t tail = 0, head = 0;

// https://stanislavs.org/helppc/make_codes.html
unsigned char lowerScancodeToAscii[128] = {

	0,   27,   '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0', '-',
	'=', '\b', '\t', 'q', 'w',  'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'[', ']',  '\n', 0,   'a',  's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',
	';', '\'', '`',  0,   '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',',
	'.', '/',  0,    '*', 0,    ' ', 0,   0,   0,   0,   0,   0,   0,
	0,   0,    0,    0,   0,    0,   0,   0,   0,   '-', 0,   0,   0,
	'+', 0,    0,    0,   0,    0,   0,   0,   0,   0,   0,   0
};

unsigned char upperScancodeToAscii[128] = {

	0,   27,   '!',  '@', '#', '$', '%', '^', '&', '*', '(', ')', '_',
	'+', '\b', 1,    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'{', '}',  '\n', 0,   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	':', '"',  '~',  0,   '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<',
	'>', '?',  0,    '*', 0,   ' ', 0,   0,   0,   0,   0,   0,   0,
	0,   0,    0,    0,   0,   0,   0,   0,   0,   '-', 0,   0,   0,
	'+', 0,    0,    0,   0,   0,   0,   0,   0,   0,   0,   0

};

int init_keyboard(void)
{
	if (open_sem(KEYBOARD_SEM_ID, 0) != SEM_SUCCESS)
		return -1;
	return 0;
}

static void append_buffer(int c)
{
	if ((head + 1) % BUFFER_SIZE != tail) {
		buffer[head++] = c;
		head %= BUFFER_SIZE;
		post_sem(KEYBOARD_SEM_ID);
	}
}

void keyboard_handler()
{
	uint8_t scancode = getKey();
	if (scancode < 0x80) { // Make/Pressed
		if (scancode == LEFT_SHIFT) {
			flags |= LEFT_SHIFT_FLAG;
		} else if (scancode == RIGHT_SHIFT) {
			flags |= RIGHT_SHIFT_FLAG;
		} else if (scancode == ESC) {
			append_buffer(EOF);
		} else {
			if ((flags & LEFT_SHIFT_FLAG) ||
			    (flags & RIGHT_SHIFT_FLAG))
				append_buffer(upperScancodeToAscii[scancode]);
			else
				append_buffer(lowerScancodeToAscii[scancode]);
		}
	} else { // Break/Released
		scancode -= 0x80;
		if (scancode == LEFT_SHIFT)
			flags &= ~LEFT_SHIFT_FLAG;
		else if (scancode == RIGHT_SHIFT)
			flags &= ~RIGHT_SHIFT_FLAG;
	}
}

int get_char(void)
{
	wait_sem(KEYBOARD_SEM_ID);
	int ans = buffer[tail++];
	tail %= BUFFER_SIZE;
	return ans;
}