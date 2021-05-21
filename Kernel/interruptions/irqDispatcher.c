#include <time.h>
#include <keyboard.h>
#include <stdint.h>
#include <naiveConsole.h>

static void int_20();
static void int_21();

void irqDispatcher(uint64_t irq) {
	// ncPrintAtt("[Debug] ",0x00, 0xFF);
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;
}

void int_20() {
	// ncPrintAtt("[Debug] TIMER", 0x00, 0xFF);
	timer_handler();
}

void int_21() {
	keyboard_handler();
}