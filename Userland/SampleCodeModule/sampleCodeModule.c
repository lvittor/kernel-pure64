/* sampleCodeModule.c */
#include <stdint.h>
#include <lib.h>
#include <utils.h>
#include <exceptions.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define MAX_COMMAND 19 // Habria que achicarlo
#define MODULES_SIZE 14

typedef void (*commandType)(void);

uint8_t counter = 0;

void beginProcess(void);
void killProcess(void);
void blockProcess(void);

static char * commandStrings[MODULES_SIZE] = {
	"help",
	"inforeg",
	"printmem",
	"printDate",
	"divisionByZero",
	"invalidOpcode",
	"printFeatures",
	"printQuadraticRoots",
	"printpid",
	"processList",
	"beginProcess",
	"killProcess",
	"blockProcess",
};
static commandType commandFunctions[MODULES_SIZE] = {
	help,
	inforeg,
	printmem,
	printDate,
	throwDivisionByZeroException,
	throwInvalidOpcodeException,
	printFeatures,
	printQuadraticRoots,
	printPid,
	processList,
	beginProcess,
	killProcess,
	blockProcess,
};

void checkModule(char * string);

void newProcess(int argc, char * argv[]) {
	while(1) {
		print_f(1, "%d\n", argc);
		for(uint64_t i = 0; i < (1L<<25); i++);
	}
}

void beginProcess(void) {
	createProcess((void *)newProcess, counter++, (char * []) {NULL});
}

void killProcess(void){
	char buffer[20] = {0};
    uint8_t pid;
	int ans;
    do {
        print_f(1, "Ingrese el pid a matar:");
        ans = get_s(buffer, 19);
    } while (ans == -1);
    
    for (int i = 0; i < ans; i++) {
        if (buffer[i] < '0' || buffer[i] > '9') {
            print_f(1, "\nNo es una direccion valida\n");
            return;
        }
    }

    sscan(buffer, "%d", &pid);
	ans = _kill(pid);
	if (ans == 0) {
    	print_f(1, "\nSe mato al proceso %d\n", pid);
	} else {
		print_f(2, "\nNo se pudo matar al proceso %d\n", pid);
	}

}

void blockProcess(void){
	char buffer[20] = {0};
    uint8_t pid;
	int ans;
    do {
        print_f(1, "Ingrese el pid a bloquear:");
        ans = get_s(buffer, 19);
    } while (ans == -1);
    
    for (int i = 0; i < ans; i++) {
        if (buffer[i] < '0' || buffer[i] > '9') {
            print_f(1, "\nNo es una direccion valida\n");
            return;
        }
    }

    sscan(buffer, "%d", &pid);
	ans = _block(pid);
	if (ans == 0) {
    	print_f(1, "\nSe bloqueo al proceso %d\n", pid);
	} else {
		print_f(2, "\nNo se pudo bloquear al proceso %d\n", pid);
	}
}

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
