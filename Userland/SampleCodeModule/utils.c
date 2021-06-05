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
    print_f(1, "Los comandos disponibles son: help, inforeg, printmem, printDate, divisionByZero, invalidOpcode.\n");
}

void printmem() {
    uint8_t arr[BYTES_TO_READ] = {0};
    print_f(1, "Ingrese la dirección a partir de la cual quiere leer: ");

    // TODO: Hacer la lectura de la dirección de memoria a leer
    uint64_t dir = 0x0000001000000000; // Habría que leer

    // Leer dirección dada por el usuario
    print_f(1, "%x\n", fillMem(dir, arr, BYTES_TO_READ));

    for (int i = 0; i < BYTES_TO_READ; i++)
        print_f(1, "%x ", arr[i]);
}
