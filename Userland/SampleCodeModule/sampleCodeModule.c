/* sampleCodeModule.c */
#include <stdint.h>
#include <lib.h>
#include <utils.h>
#include <exceptions.h>

#define BUFFER_SIZE 255 // Habria que achicarlo
#define MODULES_SIZE 6

typedef void (*commandType)(void);

char * v = (char*)0xB8000 + 79 * 2;

static int var1 = 0;
static int var2 = 0;

char buffer[BUFFER_SIZE]; // Podemos ver si hacerlo de alguna otra forma (como en keyboard.c x ejemplo)
int counter = 0;

static char * commandStrings[MODULES_SIZE] = {
	"help",
	"inforeg",
	"printmem",
	"printDate",
	"divisionByZero",
	"invalidOpcode"
};
static commandType commandFunctions[MODULES_SIZE] = {
	help,
	inforeg,
	printmem,
	printDate,
	throwDivisionByZeroException,
	throwInvalidOpcodeException
};

int main() {
	print_f(1, "Estamos en userland.\n");
	help();

	while(1) {

		// Leer hasta \n
		// Ejecutar comando

		int64_t c = getChar();
		if (c != -1){
			if (c == '\b') { // Backspace
				if (counter == 0)
					continue; // Borrar
				counter--;
			} else if (c == '\n') { // Nueva línea
				buffer[counter++] = 0;
 				checkModule(buffer);
				counter = 0;
			} else { // Letra o caracter imprimible
				buffer[counter++] = c; // Falta checkeo counter == BUFFER_SIZE
			}
			put_char(1, c);
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
}
