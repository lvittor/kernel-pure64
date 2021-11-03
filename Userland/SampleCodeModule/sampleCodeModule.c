#include <stdint.h>
#include <lib.h>
#include <utils.h>
#include <exceptions.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <test_util.h>

#define MAX_COMMAND 64 // Habria que achicarlo
#define MODULES_SIZE 14

typedef void (*func_t)(void);

typedef struct command_t {
	char *name;
	func_t function;
	char builtin;
	char *description;
} command_t;

void parseCommand(char *str);
int getCommandIndex(char *string);

void help(void);
void mem(void);
void ps(void);
void kill(void);
void nice(void);
void block(void);
void sem(void);
void pipe(void);

static command_t commands[MODULES_SIZE] = {
	{ .name = "help",
	  .function = (func_t)help,
	  .builtin = TRUE,
	  .description =
		  "muestra una lista con todos los comandos disponibles." },
	{ .name = "mem",
	  .function = (func_t)mem,
	  .builtin = TRUE,
	  .description = "Imprime el estado de la memoria." },
	{ .name = "ps",
	  .function = (func_t)ps,
	  .builtin = TRUE,
	  .description =
		  "Imprime la lista de todos los procesos con sus propiedades: nombre, ID, prioridad, stack y base pointer, foreground." },
	{ .name = "loop",
	  .function = (func_t)loop,
	  .builtin = FALSE,
	  .description =
		  "Imprime su ID con un saludo cada una determinada cantidad de segundos." },
	{ .name = "kill",
	  .function = (func_t)kill,
	  .builtin = TRUE,
	  .description = "Mata un proceso dado su ID." },
	{ .name = "nice",
	  .function = (func_t)nice,
	  .builtin = TRUE,
	  .description =
		  "Cambia la prioridad de un proceso dado su ID y la nueva prioridad." },
	{ .name = "block",
	  .function = (func_t)block,
	  .builtin = TRUE,
	  .description =
		  "Cambia el estado de un proceso entre bloqueadoy listodado su ID." },
	{ .name = "sem",
	  .function = (func_t)sem,
	  .builtin = TRUE,
	  .description =
		  "Imprime la lista de todos los semáforos con sus propiedades: estado, los procesos bloqueados en cada uno." },
	{ .name = "cat",
	  .function = (func_t)cat,
	  .builtin = FALSE,
	  .description = "Imprime el stdin tal como lo recibe." },
	{ .name = "wc",
	  .function = (func_t)wc,
	  .builtin = FALSE,
	  .description = "Cuenta la cantidad de lineas del input." },
	{ .name = "filter",
	  .function = (func_t)filter,
	  .builtin = FALSE,
	  .description = "Filtra las vocales del input." },
	{ .name = "pipe",
	  .function = (func_t)pipe,
	  .builtin = TRUE,
	  .description =
		  "Imprime la lista de todos los pipes con sus propiedades: estado, los procesos bloqueados en cada uno." },
	{ .name = "phylo",
	  .function = (func_t)wc,
	  .builtin = FALSE,
	  .description = "TODO" },
	{ .name = "test_mm",
	  .function = (func_t)test_mm,
	  .builtin = FALSE,
	  .description = "Test para el memory manager(bloqueante)." },
};

int main(int argc, char *argv[])
{
	char buffer[MAX_COMMAND + 1]; // CAMBIAAAAAAAAAAAAR

	print_f(STDOUT, ">>>Userland<<<\n");
	help();
	while (1) {
		print_f(STDERR, "\n>> ");
		int64_t ans = get_s(buffer, MAX_COMMAND);
		if (ans != -1)
			parseCommand(buffer);
		else
			print_f(1, "Invalid command\n");
	}
}

int getCommandIndex(char *string)
{
	for (int i = 0; i < MODULES_SIZE; i++)
		if (!strcmp(string, commands[i].name))
			return i;
	return -1;
}

void parseCommand(char *str)
{
	uint8_t len = strlen(str);
	char foreground = (str[len - 1] != FOREGROUND_CHAR);
	if (!foreground)
		str[len - 1] = '\0';
	char *other = str;
	int ans = strdiv(str, &other, PIPE_CHAR);
	if (ans == -1) {
		int idx = getCommandIndex(str);
		if (idx == -1) {
			print_f(STDOUT, "Invalid command \"%s\"\n", str, idx);
			return;
		}

		if (commands[idx].builtin) {
			commands[idx].function();
			return;
		}
		_process_prototype_t process = {
			.function_address = (void *)commands[idx].function,
			.name = commands[idx].name,
			.priority = 0,
			.state = READY,
			.fds = { STDIN, STDOUT, STDERR },
			.foreground = foreground,
		};
		pid_t pid = _create_process(&process, 0, (char *[]){ NULL });
		if (foreground)
			_wait(pid);

	} else {
		int index1 = getCommandIndex(str),
		    index2 = getCommandIndex(other);
		fd_t fd[2];
		if (index1 == -1 || index2 == -1) {
			print_f(STDERR, "Invalid commands\n");
			return;
		}
		if (commands[index1].builtin || commands[index2].builtin) {
			print_f(STDERR, "Cannot pipe builtin commands.");
			return;
		}
		if (_open_pipe(fd) == -1) {
			print_f(STDERR, "Couldn't open pipes\n");
			return;
		}
		_process_prototype_t leftProcess = {
			.function_address = (void *)commands[index1].function,
			.name = commands[index1].name,
			.priority = 0,
			.state = READY,
			.fds = { STDIN, fd[1], STDERR },
			.foreground = foreground,
		};
		_process_prototype_t rightProcess = {
			.function_address = (void *)commands[index2].function,
			.name = commands[index2].name,
			.priority = 0,
			.state = READY,
			.fds = { fd[0], STDOUT, STDERR },
			.foreground = foreground,
		};
		pid_t leftPID =
			_create_process(&leftProcess, 0, (char *[]){ NULL });
		pid_t rightPID =
			_create_process(&rightProcess, 0, (char *[]){ NULL });

		if (foreground) {
			_wait(leftPID);
			_close_pipe(fd[1]);
			_wait(rightPID);
			_close_pipe(fd[0]);
		}
	}
}

void help(void)
{
	print_f(STDOUT,
		"  ******** **     ** *******  ******** *******               *******    ********\n"
		" **////// /**    /**/**////**/**///// /**////**             **/////**  **////// \n"
		"/**       /**    /**/**   /**/**      /**   /**            **     //**/**       \n"
		"/*********/**    /**/******* /******* /*******     *****  /**      /**/*********\n"
		"////////**/**    /**/**////  /**////  /**///**    /////   /**      /**////////**\n"
		"       /**/**    /**/**      /**      /**  //**           //**     **        /**\n"
		" ******** //******* /**      /********/**   //**           //*******   ******** \n"
		"////////   ///////  //       //////// //     //             ///////   ////////  \n");
	print_f(STDOUT, "The available commands are:\n");
	for (uint8_t i = 0; i < MODULES_SIZE; i++)
		print_f(STDOUT, " - %s: %s\n", commands[i].name,
			commands[i].description);
}

void mem(void)
{
	_dump_mem();
}

void ps(void)
{
	_ps();
}

void kill(void)
{
	char buffer[20] = { 0 };
	pid_t pid;
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

void nice(void)
{
	char buffer[20] = { 0 };
	int ans;
	pid_t pid;
	int64_t prio;
	do {
		print_f(1, "Ingrese el pid a nicear:");
		ans = get_s(buffer, 19);
	} while (ans == -1);

	for (int i = 0; i < ans; i++) {
		if (buffer[i] < '0' || buffer[i] > '9') {
			print_f(1, "\nNo es una direccion valida\n");
			return;
		}
	}
	print_f(STDOUT, "{%s}", buffer);
	sscan(buffer, "%d", &pid);
	print_f(STDOUT, "{%d}\n", pid);
	
	do {
		print_f(1, "Ingrese la nueva prioridad[0,40):");
		ans = get_s(buffer, 19);
	} while (ans == -1);

	for (int i = 0; i < ans; i++) {
		if (buffer[i] < '0' || buffer[i] > '9') {
			print_f(1, "\nNo es una prioridad valida\n");
			return;
		}
	}
	sscan(buffer, "%d", &prio);

	ans = _nice(pid, prio);
	if (ans == 0) {
		print_f(1, "\nSe niceo al proceso %d\n", pid);
	} else {
		print_f(2, "\nNo se pudo nicear al proceso %d\n", pid);
	}
}

void block(void)
{
	char buffer[20] = { 0 };
	pid_t pid;
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

void sem(void)
{
	_dump_sems();
}

void pipe(void)
{
	_dump_pipes();
}