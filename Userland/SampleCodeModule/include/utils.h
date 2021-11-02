#ifndef __UTILS_H__
#define __UTILS_H__

#define BYTES_TO_READ 32

void help(void);
void printPid(void);
void beginProcess(int fdin, int fdout, int foreground);
void kill(void);
void block(void);
void nice(void);
void loopWrapper(int fdin, int fdout, int foreground);
void wcWrapper(int fdin, int fdout, int foreground);

#endif