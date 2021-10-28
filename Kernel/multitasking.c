#include <multitasking.h>

#define TASK_COUNT 2

void _openContext(uint64_t baseRSP);
uint64_t _buildContext(uint64_t baseRSP, uint64_t functionAddress);

typedef struct processControlBlock {
    uint64_t taskRSP;
    uint64_t functionAddress;
    uint64_t baseRSP;
} processControlBlock;

static processControlBlock tasks[TASK_COUNT];

static uint8_t currentTask = 0;

void loadTask(uint8_t id, uint64_t functionAddress, uint64_t baseRSP, prompt_info prompt) {
    if (id >= TASK_COUNT)
        return;
    
    tasks[id].baseRSP = baseRSP;
    tasks[id].functionAddress = functionAddress;
    tasks[id].taskRSP = _buildContext(baseRSP, functionAddress);
}

void initCurrentTask() {
    _openContext(tasks[currentTask].taskRSP);
}

void setCurrentRSP(uint64_t rsp) {
    tasks[currentTask].taskRSP = rsp;
}

uint64_t getCurrentRSP() {
    return tasks[currentTask].taskRSP;
}

void switchTasks() {
    currentTask = (currentTask + 1) % TASK_COUNT;
}

void rebootCurrentTask() {
    tasks[currentTask].taskRSP = _buildContext(tasks[currentTask].baseRSP, tasks[currentTask].functionAddress);
    initCurrentTask();
}