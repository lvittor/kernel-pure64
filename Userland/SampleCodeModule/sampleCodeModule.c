/* sampleCodeModule.c */
#include <stdint.h>
#include <lib.h>
#include <utils.h>
#include <exceptions.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_COMMAND 14 // Habria que achicarlo
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

void checkModule(char * string);

int main() {

	/*char c;
	int i;
	int h;
	int ret = 0;
	ret = scan("%c %x %d", &c, &i, &h);
	print_f(1, "C = %c, I = %d, H = %x, Return %d", c, i, h, ret);
	return 0;*/

	char buffer[MAX_COMMAND + 1];
	int32_t counter = 0;

	print_f(1, "Estamos en userland.\n");
	help();

	while(1) {
		print_f(2, "\n>> ");
		int64_t ans = get_s(&buffer, MAX_COMMAND);
		put_char(1, '\n');
		if (ans != -1)
			checkModule(buffer);
		else
			print_f(1, "Comando no valido\n");
	}
}

void checkModule(char * string) {
	for (int i = 0; i < MODULES_SIZE; i++){
		if (!strcmp(string, commandStrings[i])){
			commandFunctions[i]();
			return;
		}
	}
	print_f(1, "Comando no valido\n");
}
