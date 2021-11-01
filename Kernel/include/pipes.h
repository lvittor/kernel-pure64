#ifndef __PIPES_H__
#define __PIPES_H__

int initPipes(void);
int openPipe(int fd[2]);
int writePipe(int fd, char * buffer, int count);;
int readPipe(int fd, char * buffer, int count);
int closePipe(int fd);

#endif 