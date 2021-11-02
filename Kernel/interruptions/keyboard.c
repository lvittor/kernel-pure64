#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>
#include <scheduler.h>
#include <semaphore.h>

#define LEFT_SHIFT  0x2A
#define LEFT_SHIFT_FLAG 0b00000001
#define RIGHT_SHIFT 0x36
#define RIGHT_SHIFT_FLAG 0b00000010
#define LEFT_ALT 0x38

#define BUFFER_SIZE 128

#define KEYBOARD_SEM_ID "SEM_KEY"

uint8_t flags = 0;

static char buffer[BUFFER_SIZE];
uint64_t tail = 0, head = 0; // head escribe, tail lee
// {0, 0, 0, 0, 0, 0, 0, 0, 0}
//  t                       h
//              h  t


// https://stanislavs.org/helppc/make_codes.html
unsigned char lowerScancodeToAscii[128] = {

      0,   27, '1', '2', '3', '4', '5', '6', '7', '8', '9',  '0', '-', '=', 
   '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',  '[', ']', 
   '\n',    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',    
      0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',    0, '*',    
      0,  ' ',   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,    0,   0,
      0,    0,   0,   0, '-',   0,   0,   0, '+',   0,   0,   0,    0,   0, 
      0,    0,   0,   0,   0,   0    

};

unsigned char upperScancodeToAscii[128] = {

      0,   27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 
   '\b', 1, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 
   '\n',    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',    
      0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0, '*',
      0, ' ',    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,    0,   0, '-',   0,   0,   0, '+',   0,   0,   0,   0,   0, 
      0,   0,    0,   0,   0,   0    
      
};

int initKeyboard(void) {
  if (openSemaphore(KEYBOARD_SEM_ID, 0) != SEM_SUCCESS)
    return -1;
  return 0;
}

static void appendBuffer(char c) {
  if ((head+1)%BUFFER_SIZE != tail) {
    buffer[head++] = c;
    head %= BUFFER_SIZE;
    postSemaphore(KEYBOARD_SEM_ID);
  }
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
          //ncPrintChar(upperScancodeToAscii[scancode]); 
          appendBuffer(upperScancodeToAscii[scancode]);
        else
          //ncPrintChar(lowerScancodeToAscii[scancode]); 
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

int64_t getChar(void) {
  waitSemaphore(KEYBOARD_SEM_ID);
  char ans = buffer[tail++];
  tail %= BUFFER_SIZE;
  return ans;
}