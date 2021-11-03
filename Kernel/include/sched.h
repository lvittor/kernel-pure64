#ifndef __SCHED_H__
#define __SCHED_H__

#include <stdint.h>
#include <pipes.h>
#include <lib.h>

#define MAX_PROCESSES 128
#define PROCESS_STACK_SIZE 0x1000 // 4Kib
#define MAX_PRIORITY 40

typedef uint8_t pid_t;
typedef uint8_t priority_t;

typedef enum sched_ret_t {
	SCHED_ERROR = -1,
	SCHED_SUCCESS,
} sched_ret_t;

typedef enum process_state_t {
	READY = 0,
	BLOCKED,
	ERROR,
	KILLED
} process_state_t;

typedef struct _process_prototype_t {
	void *function_address;
	char *name;
	priority_t priority;
	process_state_t state;
	fd_t fds[3];
	bool_t foreground;
} _process_prototype_t;

typedef struct _process_prototype_t *process_prototype_t;

void _int20(void);
void _openProcessContext(uint64_t base_rsp);
uint64_t _buildProcessContext(uint64_t base_rsp, uint64_t function_address,
			      int argc, char *argv[]);

pid_t create_process(process_prototype_t pPP, int argc, char *argv[]);
sched_ret_t init_sched(process_prototype_t pPP, int argc, char *argv[]);
uint64_t schedule(uint64_t curr_rsp);
pid_t get_curr_pid(void);
sched_ret_t kill(pid_t pid);
sched_ret_t block(pid_t pid);
sched_ret_t unblock(pid_t pid); // no implementanda en .c

// Use only inside kernel, does not check if should yield after kill or block
void set_process_state(pid_t pid, process_state_t newState);

sched_ret_t nice(pid_t pid, priority_t newPriority);
void dump_processes(void);
void yield_process(void);
void yieldProcessTo(pid_t nextPID); // no implementada en .c
int get_process_fd(pid_t pid, fd_t fd);
sched_ret_t wait_process(pid_t pid);

// ver si devolver BOOL o char, en varias partes del codigo no nos decidimos q devolver
int is_foreground(pid_t pid);

#endif