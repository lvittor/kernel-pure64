#include <stdint.h>
#include <stdio.h>
#include <lib.h>

#define MINOR_WAIT                                                             \
	1000000 // TODO: To prevent a process from flooding the screen
#define WAIT                                                                   \
	10000000 // TODO: Long enough to see theese processes beeing run at least twice

#define HIGH_PRIO 39
#define MED_PRIO 20
#define LOW_PRIO 0

static void endless_loop(void);

static uint64_t my_getpid()
{
	return _get_pid();
}

static uint64_t my_create_process(char *name)
{
	_process_prototype_t process = {
		.function_address = (void *)endless_loop,
		.name = name,
		.priority = 0,
		.state = READY,
		.fds = { STDIN, STDOUT, STDERR },
		.foreground = 0,
	};
	return _create_process(&process, 0, (char *[]){ NULL });
}

static uint64_t my_nice(uint64_t pid, uint64_t newPrio)
{
	return _nice(pid, newPrio);
}

static uint64_t my_kill(uint64_t pid)
{
	return _kill(pid);
}

static uint64_t my_block(uint64_t pid)
{
	return _block(pid);
}

static uint64_t my_unblock(uint64_t pid)
{
	return _block(pid);
}

static void bussy_wait(uint64_t n)
{
	uint64_t i;
	for (i = 0; i < n; i++)
		;
}

static void endless_loop(void)
{
	uint64_t pid = my_getpid();

	while (1) {
		print_f(STDOUT, "%d ", pid);
		bussy_wait(MINOR_WAIT);
	}
}

#define TOTAL_PROCESSES 3

void test_prio(void)
{
	uint64_t pids[TOTAL_PROCESSES];
	uint64_t i;

	print_f(STDOUT, "Comenzando prueba...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		pids[i] = my_create_process("endless_loop");

	bussy_wait(WAIT);
	print_f(STDOUT, "\nCHANGING PRIORITIES...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		switch (i % 3) {
		case 0:
			my_nice(pids[i], LOW_PRIO); //lowest priority
			break;
		case 1:
			my_nice(pids[i], MED_PRIO); //medium priority
			break;
		case 2:
			my_nice(pids[i], HIGH_PRIO); //highest priority
			break;
		}
	}

	bussy_wait(WAIT);
	print_f(STDOUT, "\nBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		my_block(pids[i]);

	print_f(STDOUT, "CHANGING PRIORITIES WHILE BLOCKED...\n");
	for (i = 0; i < TOTAL_PROCESSES; i++) {
		switch (i % 3) {
		case 0:
			my_nice(pids[i], MED_PRIO); //medium priority
			break;
		case 1:
			my_nice(pids[i], MED_PRIO); //medium priority
			break;
		case 2:
			my_nice(pids[i], MED_PRIO); //medium priority
			break;
		}
	}

	print_f(STDOUT, "UNBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		my_unblock(pids[i]);

	bussy_wait(WAIT);
	print_f(STDOUT, "\nBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		my_block(pids[i]);

	print_f(STDOUT, "CHANGING PRIORITIES WHILE BLOCKED...\n");
	for (i = 0; i < TOTAL_PROCESSES; i++) {
		switch (i % 3) {
		case 0:
			my_nice(pids[i], LOW_PRIO); //medium priority
			break;
		case 1:
			my_nice(pids[i], LOW_PRIO); //medium priority
			break;
		case 2:
			my_nice(pids[i], LOW_PRIO); //medium priority
			break;
		}
	}

	print_f(STDOUT, "UNBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		my_unblock(pids[i]);

	bussy_wait(WAIT);
	print_f(STDOUT, "\nKILLING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++)
		my_kill(pids[i]);

	_exit();
}
