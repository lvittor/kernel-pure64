#ifndef __PIPES_H__
#define __PIPES_H__

#include <stdint.h>

typedef enum pipe_ret_t {
	PIPE_ERROR = 1,
	PIPE_SUCCESS = 0,
} pipe_ret_t;

typedef uint8_t fd_t;

pipe_ret_t init_pipes(void);
pipe_ret_t open_pipe(fd_t fd[2]);
int write_pipe(fd_t fd, char *buffer, int count);

int64_t read_pipe(fd_t fd);
pipe_ret_t close_pipe(fd_t fd);
void dump_pipes(void);

#endif