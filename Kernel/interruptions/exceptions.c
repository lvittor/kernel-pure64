#include <naiveConsole.h>

#define EXCEPTION_COUNT		32
#define ZERO_EXCEPTION_ID	0x00
#define INVALID_OPCODE_ID	0x06

static void zero_division();
static void invalid_opcode();

char * exceptionMessages[EXCEPTION_COUNT] = {
	"[Exception] Division By Zero", /* 0x00 */
	0,
	0,
	0,
	0,
	0,
	"[Exception] Invalid OpCode", /* 0x06 */
	0,
};

void exceptionDispatcher(int exception) {
	// Los registros se muestran en el dispatcher
	if (exception < 0 || exception >= EXCEPTION_COUNT)
			return;
	
	// Mostrar mensaje de qué excepción ocurrió
	char * message = exceptionMessages[exception];
	if (message != 0) {
		ncPrintAtt(message, 4, 0);
		ncNewline();
	}

	if (exception == ZERO_EXCEPTION_ID)
	 	zero_division();

	if (exception == INVALID_OPCODE_ID)
	 	invalid_opcode();
	
	// Esperar tecla
	while(1);
	// Reiniciar shell (Ojalá)
}

static void zero_division() {
	// Handler para manejar excepción
}

static void invalid_opcode() {
	// Handler para manejar excepción
}
