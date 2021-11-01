#include <utils.h>
#include <lib.h>
#include <cpuid.h>
#include <cpuidflags.h>

#define BUFFER_SIZE 16

#define ISHEXA(x) (((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F') || ISDIGIT(x))

void help() {
    print_f(1, "Los comandos disponibles son:\n");
    print_f(1, " - help: Muestra los comandos disponibles\n");
    print_f(1, " - printpid: Muestra el process id del proceso que llama\n"); // CREO QUE NO VA 
    print_f(1, " - ps: Muestra el process id de todos los procesos activos\n");
    print_f(1, " - beginProcess: \n"); // ESTA NO VA EN EL FUTURO
    print_f(1, " - kill: Mata un proceso dado su ID\n");
    print_f(1, " - block: Cambia el estado de un proceso entre bloqueado y listo dado su ID\n");
    print_f(1, " - mem: Imprime el estado de la memoria\n");
}

void printPid() {
    uint8_t ans = getPid();
    print_f(1, "PID: %d", ans);
}