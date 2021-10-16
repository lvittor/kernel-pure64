#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>

#define LEFT_SHIFT 0x2A
#define LEFT_SHIFT_FLAG 0b00000001
#define RIGHT_SHIFT 0x36
#define RIGHT_SHIFT_FLAG 0b00000010
#define LEFT_ALT 0x38

#define BUFFER_SIZE 256

uint8_t flags = 0;

typedef uint8_t BufferPtr;

static uint8_t buffer[BUFFER_SIZE] = {0};
static BufferPtr w_pointer = 0, r_pointer = 0;

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

static void appendBuffer(char c) {
  buffer[w_pointer++] = c;
  return;
}

void keyboard_handler() {
  uint8_t scancode = getKey();
  // ncPrintHex(scancode);
  if (scancode < 0x80) { // Make/Pressed
    if (scancode == LEFT_SHIFT) {
      flags |= LEFT_SHIFT_FLAG;
    } else if (scancode == RIGHT_SHIFT) {
      flags |= RIGHT_SHIFT_FLAG;
    } else {
      if ((flags & LEFT_SHIFT_FLAG) || (flags & RIGHT_SHIFT_FLAG))
        // ncPrintChar(upperScancodeToAscii[scancode]);
        appendBuffer(upperScancodeToAscii[scancode]);
      else
        // ncPrintChar(lowerScancodeToAscii[scancode]);
        appendBuffer(lowerScancodeToAscii[scancode]);
    }
  } else { // Break/Released
    scancode -= 0x80;
    if (scancode == LEFT_SHIFT)
      flags &= ~LEFT_SHIFT_FLAG;
    else if (scancode == RIGHT_SHIFT)
      flags &= ~RIGHT_SHIFT_FLAG;
  }
}

// int64_t getChar(void) {
//   if (head != tail) {
//     char ans = buffer[tail++];
//     if (tail == BUFFER_SIZE)
//       tail = 0;
//       return ans == 0 ? -1 : ans;
//   }
//   return -1;
// }

long copy_from_buffer(char *buf, size_t count) {
  if (r_pointer == w_pointer)
    return -1; // TODO: Check what we can do if the buffer "pega la vuelta".

  if (count > BUFFER_SIZE) {
  } // TODO: what do we do.

  long i = 0;

  while (i < count &&
         r_pointer != w_pointer) // TODO: Check if we keep the last \n
    buf[i++] = buffer[r_pointer++];

  return i;
}