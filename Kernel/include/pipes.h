#ifndef __PIPES_H__
#define __PIPES_H__

#include <stdint.h>

int initPipes(void);
int openPipe(int fd[2]);
int writePipe(int fd, char * buffer, int count);;
int64_t readPipe(int fd);
int closePipe(int fd);

#endif 