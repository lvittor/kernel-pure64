/* sampleCodeModule.c */
#include <stdint.h>
#include <lib.h>
#include <utils.h>
#include <exceptions.h>

#define BUFFER_SIZE 15 // Habria que achicarlo
#define MODULES_SIZE 7

typedef void (*commandType)(void);

static char * commandStrings[MODULES_SIZE] = {
	"help",
	"inforeg",
	"printmem",
	"printDate",
	"divisionByZero",
	"invalidOpcode",
	"printFeatures"
};
static commandType commandFunctions[MODULES_SIZE] = {
	help,
	inforeg,
	printmem,
	printDate,
	throwDivisionByZeroException,
	throwInvalidOpcodeException,
	printFeatures
};

int main() {
	char buffer[BUFFER_SIZE+1];
	int32_t counter = 0;

	print_f(1, "Estamos en userland.\n");
	help();
	print_f(2, "\n>> ");

	while(1) {

		// Leer hasta \n
		// Ejecutar comando

		int64_t c = getChar();
		if (c != -1) {
			if (counter < BUFFER_SIZE) {
				if (c == '\n') { // Nueva linea, proceso comando
					put_char(1, c);
					buffer[counter++] = 0;
					checkModule(buffer);
					counter = 0;
					put_char(1, c);
					print_f(2, ">> ");
				} else {
					if (c == '\b') { // Backspace
						if (counter == 0)
							continue;
						counter--;
					} else { // Letra o caracter imprimible
						buffer[counter++] = c; // Falta checkeo counter == BUFFER_SIZE
					}
					put_char(1, c);
				}
			} else {
				if (c == '\n') { // Nueva linea, proceso comando
					put_char(1, c);
					counter = 0;
					put_char(1, c);
					print_f(2, ">> ");
				} else  if (c == '\b') { // Backspace
					counter--;
					put_char(1, c);
				} else {
					counter++;
					put_char(1, c);
				}
			}
		}
	}
}

void checkModule(char * string){
	for (int i = 0; i < MODULES_SIZE; i++){
		if (!strcmp(string, commandStrings[i])){
			commandFunctions[i]();
			return;
		}
	}
	print_f(1, "Comando no válido\n");
}
