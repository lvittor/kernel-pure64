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
	"nice"
};
static commandType commandFunctions[MODULES_SIZE] = {
	help,
	printPid,
	ps,
	beginProcess,
	kill,
	block,
	_memdump,
	nice
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
			getCommand(buffer);
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

void getCommand(char * str) {
	char * other = str;
	int ans = strdiv(str, &other, '|');
	if (ans == -1) { // NO HUBO PIPEO
		return checkModule(str);
	} else {
		
	} // ESCRIBI ESTO TE AMO JAJJA SEGUI SONANDO

	
	// v en caso de que si, tocaria hacer cosas con pipes v
	// aca habria que abrir el pipe con los fds y de ahi llamar
	// a checkmodule con cada uno de los comandos. En caso de que
	// alguno no sea un comando valido checkear eso etc. 
}
