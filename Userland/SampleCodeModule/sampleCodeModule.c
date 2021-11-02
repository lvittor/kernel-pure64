#include <stdint.h>
#include <lib.h>
#include <utils.h>
#include <exceptions.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_COMMAND 19 // Habria que achicarlo
#define MODULES_SIZE 9

typedef void (*commandType)(int fdin, int fdout, int foreground);

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

void checkModule(char * string, int fdin, int fdout, int foreground);

int main(int argc, char * argv[]) {
	char buffer[MAX_COMMAND + 1]; // CAMBIAAAAAAAAAAAAR

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

void checkModule(char * string, int fdin, int fdout, int foreground) {
	for (int i = 0; i < MODULES_SIZE; i++){
		if (!strcmp(string, commandStrings[i])){
			commandFunctions[i](fdin, fdout, foreground);
			return;
		}
	}
	print_f(1, "Comando no valido\n");
}

void getCommand(char * str) {
	char * other = str;
	int ans = strdiv(str, &other, '|');
	if (ans == -1) { // NO HUBO PIPEO
		return checkModule(str, 0, 1, 0);
	} else {
		int fd[2];
		if (_open_pipe(fd) == -1)
			return;
		checkModule(other, fd[0], 1, 1); // CAMBIARLA ARRIBA. HABRIA QUE PASARLE PARAMETROS ETC. 
		checkModule(str, 0, fd[1], 0);

		_close_pipe(fd[0]);
		_close_pipe(fd[1]);
	} 
}
