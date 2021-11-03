#include <naiveConsole.h>
#include <video.h>
#include <keyboard.h>
#include <interrupts.h>
#include <sched.h>

#define EXCEPTION_COUNT 32
#define ZERO_EXCEPTION_ID 0x00
#define INVALID_OPCODE_ID 0x06

static void zero_division();
static void invalid_opcode();

char *exception_strings[EXCEPTION_COUNT] = {
	"[Exception] Division By Zero", /* 0x00 */
	0,
	0,
	0,
	0,
	0,
	"[Exception] Invalid OpCode", /* 0x06 */
	0,
};

void exceptionDispatcher(int exception)
{
	if (exception < 0 || exception >= EXCEPTION_COUNT)
		return;

	char *message = exception_strings[exception];
	if (message != 0) {
		ncPrintAtt(message, &RED, &BLACK);
		ncNewline();
		ncNewline();
	}

	if (exception == ZERO_EXCEPTION_ID)
		zero_division();

	if (exception == INVALID_OPCODE_ID)
		invalid_opcode();

	ncPrint("Presione enter para continuar\n");
	while (get_char() != '\n')
		_hlt();

	clearWindow(&BLACK);
	set_process_state(get_curr_pid(), ERROR);
	yield_process();
}

static void zero_division()
{
	// Handler para manejar excepción
}

static void invalid_opcode()
{
	// Handler para manejar excepción
}
