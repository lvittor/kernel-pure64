/* sampleCodeModule.c */
#include <exceptions.h>
#include <lib.h>
#include <stdint.h>
#include <utils.h>
#include <my_process.h>
#include <string.h>

#define MAX_COMMAND 19 // Habria que achicarlo
#define MODULES_SIZE 8

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef void (*commandType)(void);

static char *commandStrings[MODULES_SIZE] = {
    "help",           "inforeg",       "printmem",      "printDate",
    "divisionByZero", "invalidOpcode", "printFeatures", "printQuadraticRoots"};
static commandType commandFunctions[MODULES_SIZE] = {
    help,
    inforeg,
    printmem,
    printDate,
    throwDivisionByZeroException,
    throwInvalidOpcodeException,
    printFeatures,
    printQuadraticRoots};
  
void my_proc(int argc, char *argv[]) {
  print_f(1, "Soy el proceso %s\n", argv[0]);
  print_f(1, "Tengo %d argumentos\n", argc);
  print_f(1, "Mis argumentos son: \n");
  for(int i = 0; i < argc; i++) {
    print_f(1, "%s ", argv[i]);
  }
  put_char(1, '\n');
  ps();
  exit();
}

void checkModule(char *string);

int main() {
  char buffer[MAX_COMMAND + 1];
  char *argv[] = {"my_proc", "Juan", "Ignacio", "Garcia", "Matwieiszyn", NULL};
  createPs((uint64_t) &my_proc, "endless loop", 5, argv, 1);
  print_f(1, "Ingrese help para ver todos los comandos.\n");

  while (1) {
    print_f(2, "\nbareOS $> ");
    int64_t ans = get_s(buffer, MAX_COMMAND);
    if (ans != -1)
      checkModule(buffer);
    else
      print_f(1, "Comando no valido\n");
  }
}

void checkModule(char *string) {
  for (int i = 0; i < MODULES_SIZE; i++) {
    if (!strcmp(string, commandStrings[i])) {
      commandFunctions[i]();
      return;
    }
  }
  print_f(1, "Comando no valido\n");
}
