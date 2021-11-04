#ifndef __LIB_H__
#define __LIB_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <utils.h>

#define IN_RANGE(x, a, b) ((x) >= (a) && (x) <= (b))
#define ISUPPER(x) IN_RANGE(x, 'A', 'Z')
#define ISLOWER(x) IN_RANGE(x, 'a', 'z')
#define ISALPHA(x) (ISUPPER(x) || ISLOWER(x))
#define ISDIGIT(x) IN_RANGE(x, '0', '9')

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef enum process_state_t {
	READY = 0,
	BLOCKED,
	ERROR,
	KILLED
} process_state_t;

typedef uint8_t pid_t;
typedef uint8_t priority_t;
typedef uint8_t fd_t;

typedef struct _process_prototype_t {
	void *function_address;
	char *name;
	uint8_t priority;
	process_state_t state;
	fd_t fds[3];
	bool_t foreground;
} _process_prototype_t;

typedef enum sem_ret_t {
	SEM_SUCCESS = 0,
	SEM_EXISTS,
	SEM_INVALID,
	SEM_ENOMEM,
} sem_ret_t;

typedef uint8_t fd_t;
typedef uint8_t pid_t;

typedef char *semid_t;
typedef uint64_t semvalue_t;

int print_f(fd_t fd, const char *format, ...);
void put_char(fd_t fd, const char character);
void put_s(fd_t fd, const char *s);
int sscan(char *buff, char *fmt, ...);
int64_t strtoint(const char *nptr, char **endptr, register int base);
double strtodouble(const char *start, char **end);
int64_t get_s(char *buffer, uint64_t maxLength);
int atoi(const char *s);

void _exit(void);
void _clear_window(void);
void _print(fd_t fd, const char *s, uint64_t count);
int _get_char(fd_t fd);
pid_t _get_pid(void);
void _ps(void);
uint64_t _create_process(void *function_address, int argc, char *argv[]);
uint64_t _kill(pid_t pid);
uint64_t _block(pid_t pid);
void *_alloc(size_t size);
void _free(void *address);
void _dump_mem(void);
uint64_t _nice(pid_t pid, priority_t priority);
int _open_pipe(fd_t fd[2]);
int _write_pipe(fd_t fd, char *buffer, int count);
int _read_pipe(fd_t fd);
int _close_pipe(fd_t fd);
void _dump_pipes(void);
int _seconds_elapsed(void);
int _wait(pid_t pid);
void _yield_process(void);

sem_ret_t _open_sem(semid_t sid, semvalue_t value);
sem_ret_t _wait_sem(semid_t sid);
sem_ret_t _post_sem(semid_t sid);
sem_ret_t _close_sem(semid_t sid);
void _dump_sems(void);

#endif