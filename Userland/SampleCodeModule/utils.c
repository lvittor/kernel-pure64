#include <utils.h>
#include <lib.h>

void printDate() {
	dateType currDate;
	fillDate(&currDate);
	print_f(1, "Date: %d/%d/%d\nTime: %d:%d:%d (UTC)\n", currDate.day, 
                                                         currDate.month, 
                                                         currDate.year + 2000, 
                                                         currDate.hour, 
                                                         currDate.minute, 
                                                         currDate.second);
}

void help() {
    print_f(1, "Los comandos disponibles son: help, inforeg, printmem, date, divisionByZero, invalidOpCode.\n");
}

void printmem() {
    uint8_t arr[BYTES_TO_READ] = {0};
    print_f(1, "Ingrese la dirección a partir de la cual quiere leer: ");
    //512M = 512 * 2^20 = 2^29 = 1 1111 1111 1111 1111 1111 1111 1111 = 1FFFFFFF
    //16GB = 2^4 * 2^30 = 2^34 = 11 1111 1111 1111 1111 1111 1111 1111 1111 = 3FFFFFFFF

    print_f(1, "ANTES\n");

    uint64_t dir = 0x0000001000000000;

    // Leer dirección dada por el usuario
    print_f(1, "%x\n", fillMem(dir, arr, BYTES_TO_READ));

    for (int i = 0; i < BYTES_TO_READ; i++)
        print_f(1, "%x ", arr[i]);

    print_f(1, "DESPUES\n");

/*     
    print_f(1, "Memoria a partir de 0x%x: ", dir);
    for (int i = 0; i < BYTES_TO_READ; i++) {
        print_f(1, "%xh, ", arr[i]);
    }
    print_f(1, "\n");

    */
}
