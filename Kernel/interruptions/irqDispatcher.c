#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <naiveConsole.h>

#define NULL 0

void (*handlers[15])(void) = { timer_handler, // int 20h
			       keyboard_handler, // int 21h
			       NULL };

void irqDispatcher(uint64_t irq)
{
	void (*interruptionHandler)(void) = handlers[irq];
	if (interruptionHandler != NULL)
		(*interruptionHandler)();
	return;
}