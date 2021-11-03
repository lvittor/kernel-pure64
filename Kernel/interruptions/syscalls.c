#include <syscalls.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <stdlib.h>
#include <lib.h>
#include <keyboard.h>
#include <video.h>
#include <sched.h>
#include <mm.h>
#include <sem.h>
#include <pipes.h>
#include <time.h>

#define MAX_MEMORY_MAPPED 0x1000000000
#define MAX_SYSCALLS 25

typedef struct dateType {
	uint8_t year, month, day;
	uint8_t hour, minute, second;
} dateType;

uint64_t sys_exit(void);

uint64_t sys_write(fd_t fd, char *buffer, uint64_t count);
int64_t sys_read(fd_t fd);

pid_t sys_get_curr_pid(void);
void sys_dump_processes(void);
pid_t sys_create_process(process_prototype_t pPP, int argc, char *argv[]);
sched_ret_t sys_kill(pid_t pid);
sched_ret_t sys_block(pid_t pid);
sched_ret_t sys_nice(pid_t pid, priority_t priority);

void *sys_alloc(size_t size);
void sys_free(void *address);
void sys_dump_mem(void);

sem_ret_t sys_open_sem(semid_t sid, semvalue_t value);
sem_ret_t sys_wait_sem(semid_t sid);
sem_ret_t sys_post_sem(semid_t sid);
sem_ret_t sys_close_sem(semid_t sid);
void sys_dump_sems(void);

pipe_ret_t
sys_open_pipe(fd_t fd[2]); // falta agregar la syscall para init_pipes
int sys_write_pipe(fd_t fd, char *buffer, int count);
int sys_read_pipe(fd_t fd); // en pipes.h esta como int64_t
pipe_ret_t sys_close_pipe(fd_t fd);
void sys_dump_pipes(void);

int sys_seconds_elapsed(void);
int sys_wait_process(uint8_t pid);
void sys_yield_process(void);

typedef int64_t (*syscall)(int64_t, int64_t, int64_t);

syscall syscalls[MAX_SYSCALLS] = {
	(syscall)sys_exit,
	(syscall)sys_write,
	(syscall)sys_read,
	(syscall)sys_get_curr_pid,
	(syscall)sys_dump_processes,
	(syscall)sys_create_process,
	(syscall)sys_kill,
	(syscall)sys_block,
	(syscall)sys_alloc,
	(syscall)sys_free,
	(syscall)sys_dump_mem,
	(syscall)sys_nice,
	(syscall)sys_open_sem,
	(syscall)sys_wait_sem,
	(syscall)sys_post_sem,
	(syscall)sys_close_sem,
	(syscall)sys_dump_sems,
	(syscall)sys_open_pipe,
	(syscall)sys_write_pipe,
	(syscall)sys_read_pipe,
	(syscall)sys_close_pipe,
	(syscall)sys_dump_pipes,
	(syscall)sys_seconds_elapsed,
	(syscall)sys_wait_process,
	(syscall)sys_yield_process,
};

uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx,
			   uint64_t rcx)
{
	syscall sc;
	if (rcx >= MAX_SYSCALLS || (sc = syscalls[rcx]) == NULL)
		return -1;

	return sc(rdi, rsi, rdx);
}

uint64_t sys_exit(void)
{
	return kill(get_curr_pid());
}

pid_t sys_get_curr_pid(void)
{
	return get_curr_pid();
}

uint64_t sys_write(fd_t fd, char *buffer, uint64_t count)
{
	if (buffer == NULL || count <= 0)
		return -1;
	if (fd < 3)
		fd = get_process_fd(get_curr_pid(), fd);

	if (fd == STDOUT || fd == STDERR) {
		Color *fontColor = (fd == STDERR) ? &RED : &WHITE;
		for (int i = 0; i < count && buffer[i]; i++)
			ncPrintCharAtt(buffer[i], fontColor, &BLACK);
		return count;
	}

	return write_pipe(fd, buffer, count);
}

int64_t sys_read(fd_t fd)
{
	if (fd < 3)
		fd = get_process_fd(get_curr_pid(), fd);
	if (fd == STDIN)
		return is_foreground(get_curr_pid()) ? get_char() : -1;
	return read_pipe(fd);
}

void sys_dump_processes(void)
{
	dump_processes();
}

pid_t sys_create_process(process_prototype_t pPP, int argc, char *argv[])
{
	return create_process(pPP, argc, argv);
}

sched_ret_t sys_kill(pid_t pid)
{
	return kill(pid);
}

sched_ret_t sys_block(pid_t pid)
{
	return block(pid);
}

void *sys_alloc(size_t size)
{
	return alloc(size);
}

void sys_free(void *address)
{
	free(address);
}

void sys_dump_mem(void)
{
	dump_mem();
}

sched_ret_t sys_nice(pid_t pid, priority_t priority)
{
	return nice(pid, priority);
}

sem_ret_t sys_open_sem(semid_t sid, semvalue_t value)
{
	return open_sem(sid, value);
}

sem_ret_t sys_wait_sem(semid_t sid)
{
	return wait_sem(sid);
}

sem_ret_t sys_post_sem(semid_t sid)
{
	return post_sem(sid);
}

sem_ret_t sys_close_sem(semid_t sid)
{
	return close_sem(sid);
}

void sys_dump_sems(void)
{
	dump_sems();
}

pipe_ret_t sys_open_pipe(fd_t fd[2])
{
	return open_pipe(fd);
}

int sys_write_pipe(fd_t fd, char *buffer, int count)
{
	return write_pipe(fd, buffer, count);
}

int sys_read_pipe(fd_t fd)
{
	return read_pipe(fd);
}

pipe_ret_t sys_close_pipe(fd_t fd)
{
	return close_pipe(fd);
}

void sys_dump_pipes(void)
{
	dump_pipes();
}

int sys_seconds_elapsed(void)
{
	return seconds_elapsed();
}

int sys_wait_process(uint8_t pid)
{
	return wait_process(pid);
}

void sys_yield_process(void)
{
	yield_process();
}
