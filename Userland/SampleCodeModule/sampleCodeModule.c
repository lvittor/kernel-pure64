/* sampleCodeModule.c */
#include <exceptions.h>
#include <lib.h>
#include <stdint.h>
#include <utils.h>

#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND 19 // Habria que achicarlo
#define MODULES_SIZE 8

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

void checkModule(char *string);
void endlessLoop(int argc, char *argv[]){
  print_f(1, "Cantidad de argumentos recibidos: %d\n", argc);
  for(int i = 0; argv[i] != NULL; i++)
    print_f(1, "%s ", argv[i]);
  put_char(1, '\n');
  while(1);
}

int main() {
  char buffer[MAX_COMMAND + 1];
  char *argv[] = {"Juan", "Ignacio", "Garcia", "Budweiser", NULL};
  pid_t pid = createPs((uint64_t)&endlessLoop, 24, argv);
  print_f(1, "Created process pid: %d\n", pid);
  ps();
  print_f(1, "Ingrese help para ver todos los comandos.\n");

  while (1) {
    print_f(2, "\n>> ");
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
