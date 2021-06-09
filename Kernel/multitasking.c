#include <multitasking.h>

#define TASK_COUNT 2

typedef struct processControlBlock {
    uint64_t taskRSP;
    uint64_t functionAddress;
    uint64_t baseRSP;
    // prompt_info
} processControlBlock;

static processControlBlock tasks[TASK_COUNT];

static uint8_t currentTask = 0;

void loadTask(uint8_t id, uint64_t functionAddress, uint64_t baseRSP) {
    if (id >= TASK_COUNT)
        return;
    
    tasks[id].functionAddress = functionAddress;
    tasks[id].baseRSP = baseRSP;
    tasks[id].taskRSP = _buildContext(baseRSP, functionAddress);
}

void initTasks() {
    _openContext(tasks[0].taskRSP);
}

void setCurrRSP(uint64_t rsp) {
    tasks[currentTask].taskRSP = rsp;
}

uint64_t getCurrRSP() {
    return tasks[currentTask].taskRSP;
}

void switchTasks() {
    currentTask = (currentTask + 1) % TASK_COUNT;
}


void rebootTask() {
    /*
    if (currentTask == 1) {
        task1RSP = 0x600000;
        create_task(1, 0x600000, 0x400000);
    } else {
        task1RSP = 0x700000;
        create_task(2, 0x700000, 0x400000);
    }
    */
}

