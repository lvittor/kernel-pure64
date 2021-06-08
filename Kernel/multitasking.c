#include <multitasking.h>

uint8_t currentTask = 0;
uint64_t task1RSP = 0;
uint64_t task2RSP = 0;


void rebootTask() {
    if (currentTask == 1) {
        task1RSP = 0x600000;
        create_task(1, 0x600000, 0x400000);
    } else {
        task1RSP = 0x700000;
        create_task(2, 0x700000, 0x400000);
    }
}

void switchTasks() {
    if (currentTask == 1) { // Me paso a la 2
        task1RSP = getRSP();
        currentTask = 2;
        if (task2RSP == 0)
            create_task(2, 0x700000, 0x400000);
        else
            setRSP(task2RSP);    
    } else { // Vuelvo a la 1
        task2RSP = getRSP();
        currentTask = 1;
        if (task1RSP == 0)
            create_task(1, 0x600000, 0x400000);
        else 
            setRSP(task1RSP);
        
    }
}