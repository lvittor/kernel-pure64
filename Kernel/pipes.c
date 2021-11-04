// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipes.h>
#include <mm.h>
#include <sched.h>
#include <sem.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <lib.h>

#define MAX_PIPES 10
#define SEM_ID_SIZE 3 // Should be able to contain itoa(MAX_PIPES*2)
#define TOTAL_FDS (MAX_PIPES * 2 + 3)
#define MAX_BUFFER 256

typedef struct pipe_t {
	fd_t fdin;
	fd_t fdout;
	int buffer[MAX_BUFFER];
	uint16_t read_pos;
	uint16_t write_pos;
	char write_sem[SEM_ID_SIZE], read_sem[SEM_ID_SIZE];
} pipe_t;

static pipe_t *pipes[MAX_PIPES];
static int fd_to_pipe[TOTAL_FDS];

pipe_ret_t init_pipes(void)
{
	for (int i = 0; i < 3; i++)
		fd_to_pipe[i] = MAX_PIPES;
	for (int i = 3; i < TOTAL_FDS; i++)
		fd_to_pipe[i] = -1;
	return PIPE_SUCCESS;
}

static int is_valid_fd(fd_t fd)
{
	return fd < TOTAL_FDS;
}

static int get_free_pipe_idx(void)
{
	for (int i = 0; i < MAX_PIPES; i++) {
		if (pipes[i] == NULL)
			return i;
	}
	return PIPE_ERROR;
}

static int get_next_fd()
{
	for (int i = 0; i < TOTAL_FDS; i++) {
		if (fd_to_pipe[i] == -1)
			return i;
	}
	return PIPE_ERROR;
}

// Writes the string value of i into buffer, backwards: i(10) -> buffer("01")
static void itoa(char *buffer, int i)
{
	buffer[0] = '0';
	while (i > 0) {
		buffer[0] = '0' + i % 10;
		i /= 10;
		buffer++;
	}
	buffer[0] = '\0';
}

pipe_ret_t open_pipe(fd_t fd[2])
{
	int pipe_idx = get_free_pipe_idx();
	if (pipe_idx == PIPE_ERROR)
		return PIPE_ERROR;

	pipes[pipe_idx] = alloc(sizeof(pipe_t));

	int fd1 = get_next_fd();
	if (fd1 == PIPE_ERROR) {
		free(pipes[pipe_idx]);
		pipes[pipe_idx] = NULL;
		return PIPE_ERROR;
	}
	pipes[pipe_idx]->fdin = fd1;
	fd_to_pipe[fd1] = pipe_idx;

	int fd2 = get_next_fd();
	if (fd2 == PIPE_ERROR) {
		free(pipes[pipe_idx]);
		pipes[pipe_idx] = NULL;
		fd_to_pipe[fd1] = -1;
		return PIPE_ERROR;
	}

	pipes[pipe_idx]->fdout = fd2;
	fd_to_pipe[fd2] = pipe_idx;

	pipes[pipe_idx]->read_pos = 0;
	pipes[pipe_idx]->write_pos = 0;

	itoa(pipes[pipe_idx]->read_sem, pipes[pipe_idx]->fdin);
	itoa(pipes[pipe_idx]->write_sem, pipes[pipe_idx]->fdout);
	int ans = open_sem(pipes[pipe_idx]->read_sem, 0);
	if (ans != SEM_SUCCESS) {
		free(pipes[pipe_idx]);
		pipes[pipe_idx] = NULL;
		fd_to_pipe[fd1] = -1;
		fd_to_pipe[fd2] = -1;
		return PIPE_ERROR;
	}

	ans = open_sem(pipes[pipe_idx]->write_sem, MAX_BUFFER);
	if (ans != SEM_SUCCESS) {
		close_sem(pipes[pipe_idx]->read_sem);
		free(pipes[pipe_idx]);
		pipes[pipe_idx] = NULL;
		fd_to_pipe[fd1] = -1;
		fd_to_pipe[fd2] = -1;
		return PIPE_ERROR;
	}

	fd[0] = pipes[pipe_idx]->fdin;
	fd[1] = pipes[pipe_idx]->fdout;

	return PIPE_SUCCESS;
}

int write_pipe(fd_t fd, char *buffer, int count)
{
	if (!is_valid_fd(fd) || count < 0 || buffer == NULL)
		return PIPE_ERROR;
	int pipe_idx = fd_to_pipe[fd];
	if (pipe_idx == -1 || pipes[pipe_idx]->fdout != fd)
		return PIPE_ERROR;
	for (int i = 0; i < count; i++) {
		if (wait_sem(pipes[pipe_idx]->write_sem) != SEM_SUCCESS)
			return i;
		pipes[pipe_idx]->buffer[pipes[pipe_idx]->write_pos++] =
			buffer[i];
		pipes[pipe_idx]->write_pos %= MAX_BUFFER;
		if (post_sem(pipes[pipe_idx]->read_sem) != SEM_SUCCESS)
			return i;
	}
	return count;
}

int64_t read_pipe(fd_t fd)
{
	if (!is_valid_fd(fd))
		return PIPE_ERROR;
	int pipe_idx = fd_to_pipe[fd];
	if (pipe_idx == -1 || pipes[pipe_idx]->fdin != fd)
		return PIPE_ERROR;
	if (wait_sem(pipes[pipe_idx]->read_sem) != SEM_SUCCESS)
		return PIPE_ERROR;
	if (pipes[pipe_idx]->fdout == (fd_t)-1 &&
	    pipes[pipe_idx]->read_pos == pipes[pipe_idx]->write_pos)
		return -1;
	int c = pipes[pipe_idx]->buffer[pipes[pipe_idx]->read_pos++];
	pipes[pipe_idx]->read_pos %= MAX_BUFFER;
	if (post_sem(pipes[pipe_idx]->write_sem) != SEM_SUCCESS)
		return PIPE_ERROR;
	return c;
}

pipe_ret_t close_pipe(fd_t fd)
{
	int idx = fd_to_pipe[fd];
	if (idx == -1)
		return PIPE_ERROR;
	if (pipes[idx]->fdin == fd) {
		fd_to_pipe[pipes[idx]->fdin] = -1;
		pipes[idx]->fdin = -1;
	} else {
		fd_to_pipe[pipes[idx]->fdout] = -1;
		pipes[idx]->fdout = -1;
		post_sem(pipes[idx]->read_sem);
	}

	if (pipes[idx]->fdin == (fd_t)-1 && pipes[idx]->fdout == (fd_t)-1) {
		close_sem(pipes[idx]->read_sem);
		close_sem(pipes[idx]->write_sem);
		free(pipes[idx]);
		pipes[idx] = NULL;
	}
	return PIPE_SUCCESS;
}

void dump_pipes(void)
{
	ncPrint("Pipes\n");
	for (int i = 0; i < MAX_PIPES; i++) {
		if (pipes[i] != NULL) {
			ncPrint("Pipe ");
			ncPrintDec(i);
			ncPrint(": ");
			ncPrint("fdin: ");
			ncPrintDec(pipes[i]->fdin);
			ncPrint(", ");
			ncPrint("fdout: ");
			ncPrintDec(pipes[i]->fdout);
			ncPrint(", ");
			ncPrint("read_pos: ");
			ncPrintDec(pipes[i]->read_pos);
			ncPrint(", ");
			ncPrint("write_pos: ");
			ncPrintDec(pipes[i]->write_pos);
			ncPrint(", ");
			ncPrint("write_sem: \"");
			ncPrint(pipes[i]->write_sem);
			ncPrint("\", ");
			ncPrint("read_sem: \"");
			ncPrint(pipes[i]->read_sem);
			ncPrint("\"\n");
		}
	}
	ncNewline();
	return;
}