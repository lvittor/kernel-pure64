#include <naiveConsole.h>

#define EXCEPTION_COUNT		32
#define ZERO_EXCEPTION_ID	0x00
#define INVALID_OPCODE_ID	0x06

static void zero_division();
char * exceptionMessages[EXCEPTION_COUNT] = {
	"Division By Zero Exception", /* 0x00 */
	0,
	0,
	0,
	0,
	0,
	"Invalid OpCode Exception", /* 0x06 */
	0,
};

void exceptionDispatcher(int exception) {
	// Los registros se muestran en el dispatcher
	if (exception < 0 || exception >= EXCEPTION_COUNT)
			return;
	
	// Mostrar mensaje de qué excepción ocurrió
	char * message = exceptionMessages[exception];
	if (message != 0) {
		ncPrint(message);
		ncNewline();
	}

	if (exception == ZERO_EXCEPTION_ID)
	 	zero_division();

	
	// Esperar tecla
	while(1);
	// Reiniciar shell (Ojalá)
}

static void zero_division() {
	// Handler para manejar excepción
}