#include <sched.h>
#include <lib.h>
#include <mm.h>
#include <interrupts.h>
#include <naiveConsole.h>
#include <queue.h>
#include <string.h>

typedef struct _pcb_t { // Process Control Block
	char *name;
	process_state_t state;
	uint64_t function_address;
	uint64_t curr_rsp;
	uint64_t stack_top;
	uint64_t base_rsp;
	priority_t priority;
	fd_t fds[3];
	bool_t foreground;
	queue_t waiting_queue;
} _pcb_t;

typedef struct _pcb_t *pcb_t;

static char *process_state_string[] = {
	"READY",
	"BLOCKED",
	"ERROR",
	"KILLED",
};

static char *process_foreground_string[] = {
	"BACK",
	"FORE",
};

static pcb_t processes[MAX_PROCESSES] = { NULL };
static pid_t curr_pid = 0;
static pid_t halt_pid = 0;
static pid_t desired_pid = 0;
static uint8_t tickets = 0;

static void free_process(pid_t pid);
static sched_ret_t get_tickets(pid_t pid);

void halt_process(void)
{
	while (1) {
		_hlt();
	}
}

sched_ret_t init_sched(process_prototype_t pPP, int argc, char *argv[])
{
	_process_prototype_t halt_prototype = {
		.function_address = (void *)halt_process,
		.name = "halt",
		.priority = 0,
		.state = READY,
		.fds = { 0, 1, 2 },
		.foreground = 0,
	};
	halt_pid = create_process(&halt_prototype, 0, (char *[]){ NULL });
	if (halt_pid == SCHED_ERROR)
		return SCHED_ERROR;

	desired_pid = halt_pid;

	int new_pid = create_process(pPP, argc, argv);
	if (new_pid == SCHED_ERROR)
		return SCHED_ERROR;

	curr_pid = new_pid;
	_openProcessContext(processes[curr_pid]->curr_rsp);
	return SCHED_SUCCESS;
}

pid_t create_process(process_prototype_t pPP, int argc, char *argv[])
{
	pid_t pid = 0;
	for (; pid < MAX_PROCESSES && processes[pid] != NULL; pid++)
		if (processes[pid]->state == KILLED)
			free_process(pid);
	if (pid == MAX_PROCESSES)
		return SCHED_ERROR;

	pcb_t p_pcb = alloc(sizeof(_pcb_t));
	if (p_pcb == NULL)
		return SCHED_ERROR;
	uint64_t stack_top = (uint64_t)alloc(PROCESS_STACK_SIZE);
	if (stack_top == (uint64_t)NULL) {
		free(p_pcb);
		return SCHED_ERROR;
	}
	p_pcb->name = alloc(strlength(pPP->name) + 1);
	if (p_pcb->name == NULL) {
		free((void *)stack_top);
		free(p_pcb);
		return SCHED_ERROR;
	}
	p_pcb->waiting_queue = queue_init();
	if (p_pcb->waiting_queue == NULL) {
		free(p_pcb->name);
		free((void *)stack_top);
		free(p_pcb);
		return SCHED_ERROR;
	}

	strcopy(p_pcb->name, pPP->name);
	p_pcb->name[strlength(pPP->name)] = '\0';
	p_pcb->stack_top = stack_top;
	p_pcb->state = pPP->state;
	p_pcb->base_rsp = stack_top + PROCESS_STACK_SIZE;
	p_pcb->function_address = (uint64_t)pPP->function_address;
	p_pcb->curr_rsp = _buildProcessContext(
		p_pcb->base_rsp, p_pcb->function_address, argc, argv);
	p_pcb->priority = pPP->priority;
	for (int i = 0; i < 3; i++)
		p_pcb->fds[i] = pPP->fds[i];
	p_pcb->foreground = pPP->foreground;

	processes[pid] = p_pcb;
	return pid;
}

static char is_valid_pid(pid_t pid)
{
	return pid < MAX_PROCESSES && pid != halt_pid && processes[pid] != NULL;
}

static char is_in_state(pid_t pid, process_state_t state)
{
	return is_valid_pid(pid) && processes[pid]->state == state;
}

static char is_schedulable(pid_t pid)
{
	return is_in_state(pid, READY) && pid != halt_pid;
}

static pid_t get_next_ready_pid(pid_t curr_pid)
{
	for (pid_t step = 0; step < MAX_PROCESSES; step++) {
		pid_t nextPID = (curr_pid + step + 1) % MAX_PROCESSES;
		if (is_in_state(nextPID, KILLED)) {
			free_process(nextPID);
		} else if (is_schedulable(nextPID)) {
			return nextPID;
		}
		// else BLOCKED OR ERROR
	}
	return halt_pid;
}

uint64_t schedule(uint64_t curr_rsp)
{
	processes[curr_pid]->curr_rsp = curr_rsp;
	if (tickets == 0 || !is_in_state(curr_pid, READY)) {
		if (is_schedulable(desired_pid)) {
			curr_pid = desired_pid;
			desired_pid = halt_pid;
		} else
			curr_pid = get_next_ready_pid(curr_pid);
		tickets = get_tickets(curr_pid);
	} else
		tickets--;
	return processes[curr_pid]->curr_rsp;
}

static void free_process(pid_t pid)
{
	while (!is_empty(processes[pid]->waiting_queue)) {
		queue_value_t wakeup = 0;
		if (peek(processes[pid]->waiting_queue, &wakeup))
			block(wakeup);
		dequeue(processes[pid]->waiting_queue);
	}
	free((void *)processes[pid]->waiting_queue);
	free((void *)processes[pid]->name);
	free((void *)processes[pid]->stack_top);
	free((void *)processes[pid]);
	processes[pid] = NULL;
}

pid_t get_curr_pid(void)
{
	return curr_pid;
}

void yield_process(void)
{
	tickets = 0;
	_int20();
}

void yield_process_to(pid_t pid)
{
	desired_pid = pid;
	yield_process();
}

sched_ret_t kill(pid_t pid)
{
	if (!is_valid_pid(pid))
		return SCHED_ERROR;

	processes[pid]->state = KILLED;
	if (pid == get_curr_pid()) {
		yield_process();
	}

	return SCHED_SUCCESS;
}

sched_ret_t block(pid_t pid)
{
	if (!is_valid_pid(pid))
		return SCHED_ERROR;

	if (processes[pid]->state == BLOCKED)
		processes[pid]->state = READY;
	else if (processes[pid]->state == READY)
		processes[pid]->state = BLOCKED;

	if (pid == get_curr_pid())
		yield_process();

	return SCHED_SUCCESS;
}

static char is_valid_priority(priority_t priority)
{
	return priority >= 0 && priority < MAX_PRIORITY;
}

sched_ret_t nice(pid_t pid, priority_t newPriority)
{
	if (!is_valid_pid(pid))
		return SCHED_ERROR;

	if (!is_valid_priority(newPriority))
		return SCHED_ERROR;

	processes[pid]->priority = newPriority;
	return SCHED_SUCCESS;
}

static sched_ret_t get_tickets(pid_t pid)
{
	if (!is_valid_pid(pid))
		return SCHED_ERROR;

	return processes[pid]->priority + 1;
}

void dump_processes(void)
{
	ncPrint("\nNOMBRE    PID    PRIO    STACK    BASE    MODE    STATE    FDS\n");
	for (int pid = 0; pid < MAX_PROCESSES; pid++) {
		if (processes[pid] != NULL) {
			ncPrint(processes[pid]->name);
			for (int len = strlength(processes[pid]->name); len < 6;
			     len++)
				ncPrint(" ");
			ncPrint("    ");
			int aux = pid / 10;
			int numlen = 1;
			while (aux > 0) {
				aux /= 10;
				numlen++;
			}
			ncPrintDec(pid);
			for (; numlen < 7; numlen++)
				ncPrint(" ");
			ncPrintDec(processes[pid]->priority);
			ncPrint("       ");
			ncPrintHex(processes[pid]->curr_rsp);
			ncPrint("   ");
			ncPrintHex(processes[pid]->base_rsp);
			ncPrint("  ");
			ncPrint(process_foreground_string[processes[pid]
								  ->foreground]);
			ncPrint("     ");
			ncPrint(process_state_string[processes[pid]->state]);
			ncPrint("   ");
			ncPrint("[");
			ncPrintDec(processes[pid]->fds[0]);
			ncPrint(", ");
			ncPrintDec(processes[pid]->fds[1]);
			ncPrint(", ");
			ncPrintDec(processes[pid]->fds[2]);
			ncPrint("]");
			ncPrintChar('\n');
		}
	}
}

void set_process_state(pid_t pid, process_state_t newState)
{
	if (!is_valid_pid(pid))
		return;
	processes[pid]->state = newState;
}

int get_process_fd(pid_t pid, fd_t fd)
{
	if (!is_valid_pid(pid) || fd < 0 || fd >= 3)
		return SCHED_ERROR;
	return processes[pid]->fds[fd];
}

sched_ret_t wait_process(pid_t pid)
{
	if (!is_valid_pid(pid))
		return SCHED_ERROR;
	if (is_in_state(pid, KILLED))
		return SCHED_SUCCESS;
	if (!enqueue(processes[pid]->waiting_queue, get_curr_pid()))
		return SCHED_ERROR;
	block(get_curr_pid());
	return SCHED_SUCCESS;
}

int is_foreground(pid_t pid)
{
	return is_valid_pid(pid) && processes[pid]->foreground;
}