#include <utils.h>
#include <lib.h>
#include <cpuid.h>
#include <cpuidflags.h>

#define BUFFER_SIZE 16

#define ISHEXA(x) (((x) >= 'a' && (x) <= 'f') || ((x) >= 'A' && (x) <= 'F') || ISDIGIT(x))

uint8_t counter = 0;

static void sleep(int seconds){
    int start = _secondsElapsed();
    while(1){
        int currentSeconds = _secondsElapsed();
        if(currentSeconds - start >= seconds)
            return;
    }
}

static void loop(void){
    while(1){
        uint8_t pid = getPid();
        print_f(1, "\nMi PID es: %d\n", pid);
        sleep(3);
    }
    _kill(getPid());
}

void loopWrapper(int fdin, int fdout, int foreground) {
    processPrototype loopPrototype = {
        .functionAddress = (void *) loop,
        .name = "loop",
        .priority = 0,
        .state = READY,
        .fds = {fdin, fdout, 2},
        .foreground = foreground,
    };
    
    createProcess(&loopPrototype, counter++, (char * []) {NULL});
}

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

void newProcess(int argc, char * argv[]) {
	while(1) {
		print_f(1, "Userland: %d\n", argc);
		for(uint64_t i = 0; i < (1L<<25); i++);
	}
}

void beginProcess(int fdin, int fdout, int foreground) {
    processPrototype procPrototype = {
        .functionAddress = (void *) newProcess,
        .name = "process",
        .priority = 0,
        .state = READY,
        .fds = {fdin, fdout, 2},
        .foreground = foreground,
    };
	createProcess(&procPrototype, counter++, (char * []) {NULL});
}

void kill(void){
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

void block(void){
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

void nice(void){
	char buffer[20] = {0};
    uint8_t pid;
	int ans;
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

    sscan(buffer, "%d", &pid);
	ans = _nice(pid, 30);
	if (ans == 0) {
    	print_f(1, "\nSe niceo al proceso %d\n", pid);
	} else {
		print_f(2, "\nNo se pudo nicear al proceso %d\n", pid);
	}
}