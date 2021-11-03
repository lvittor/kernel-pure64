#ifndef __UTILS_H__
#define __UTILS_H__

#define BYTES_TO_READ 32

#define FOREGROUND_CHAR '&'
#define PIPE_CHAR '|'

typedef enum bool_t { FALSE = 0, TRUE } bool_t;

void loop(int argc, char *argv[]);
void cat(int argc, char *argv[]);
void wc(int argc, char *argv[]);
void filter(int argc, char *argv[]);

#endif