#include <keyboard.h>
#include <lib.h>
#include <naiveConsole.h>

#define LEFT_SHIFT  0x2A
#define LEFT_SHIFT_FLAG 0b00000001
#define RIGHT_SHIFT 0x36
#define RIGHT_SHIFT_FLAG 0b00000010

//#define BUFFER_SIZE 128
//static char buffer[BUFFER_SIZE];



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
   '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 
   '\n',    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',    
      0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?',   0, '*',
      0, ' ',    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,    0,   0, '-',   0,   0,   0, '+',   0,   0,   0,   0,   0, 
      0,   0,    0,   0,   0,   0    
      
};

uint8_t flags = 0;

void keyboard_handler() {
    uint8_t scancode = getKey();
    if (scancode < 0x80) { // Make/Pressed
      if (scancode == LEFT_SHIFT) {
        flags |= LEFT_SHIFT_FLAG;
      } else if (scancode == RIGHT_SHIFT) {
        flags |= RIGHT_SHIFT_FLAG;
      } else {
        if ((flags & LEFT_SHIFT_FLAG) || (flags & RIGHT_SHIFT_FLAG))
          ncPrintChar(upperScancodeToAscii[scancode]); // en vez de imprimir habria que guardarlo en un buffer
          //appendBuffer(upperScancodeToAscii[scancode]);
        else
          ncPrintChar(lowerScancodeToAscii[scancode]); // en vez de imprimir habria que guardarlo en un buffer
          //appendBuffer(lowerScancodeToAscii[scancode]);
      }
    } else { // Break/Released
      scancode -= 0x80;
      if (scancode == LEFT_SHIFT)
        flags &= ~LEFT_SHIFT_FLAG;
      else if (scancode == RIGHT_SHIFT) 
        flags &= ~RIGHT_SHIFT_FLAG;
    }    
}