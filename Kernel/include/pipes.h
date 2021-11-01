#ifndef __PIPES_H__
#define __PIPES_H__

#include <memoryManager.h>
#include <scheduler.h>

int pipeOpen(int fd[2]);

int pipeClose(int fd);

int pipeRead(int fd, int length, char *buffer);

int pipeWrite(int fd, int length, char *buffer);


#endif 