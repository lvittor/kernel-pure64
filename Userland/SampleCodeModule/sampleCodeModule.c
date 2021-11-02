#include <stdint.h>
#include <lib.h>
#include <utils.h>
#include <exceptions.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_COMMAND 19 // Habria que achicarlo
#define MODULES_SIZE 8

typedef void (*commandType)(void);

void getCommand(char * str);

static char * commandStrings[MODULES_SIZE] = {
	"help",
	"printpid",
	"ps",
	"beginProcess",
	"kill",
	"block",
	"mem",
	"nice",
	"loop"
};
static commandType commandFunctions[MODULES_SIZE] = {
	(commandType)help,
	(commandType)printPid,
	(commandType)ps,
	(commandType)beginProcess,
	(commandType)kill,
	(commandType)block,
	(commandType)_memdump,
	(commandType)nice,
	(commandType)loopWrapper
};

void checkModule(char * string);

int main(int argc, char * argv[]) {
	char buffer[MAX_COMMAND + 1];

	print_f(1, "Estamos en userland.\n");
	help();
	while(1) {
		print_f(2, "\n>> ");
		int64_t ans = get_s(buffer, MAX_COMMAND);
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
