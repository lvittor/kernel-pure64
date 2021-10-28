#ifndef __MULTITASKING_H_
#define __MULTITASKING_H_

#include <stdint.h>
#include <video.h>

void loadTask(uint8_t id, uint64_t functionAddress, uint64_t baseRSP);
void initCurrentTask(void);
void setCurrentRSP(uint64_t rsp);
uint64_t getCurrentRSP(void);
void switchTasks(void);
void rebootCurrentTask(void);
prompt_info * getCurrentPrompt(void);

#endif