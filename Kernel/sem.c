#include <sem.h>
#include <stddef.h>
#include <mm.h>
#include <sched.h>
#include <queue.h>
#include <lib.h>
#include <naiveConsole.h>

static lock_t sems_lock = 0;
struct sem_t *sems[MAX_SEMS] = { NULL };

typedef struct sem_t {
	char *name;
	lock_t lock;
	uint8_t creator_process;
	semvalue_t value;
	queue_t waiting_queue;
} sem_t;

static uint8_t get_idx(semid_t sid)
{
	for (int i = 0; i < MAX_SEMS; i++) {
		if (sems[i] != NULL && strcmp(sems[i]->name, sid) == 0) {
			return i;
		}
	}
	return MAX_SEMS;
}

static uint8_t get_free_idx()
{
	for (int i = 0; i < MAX_SEMS; i++) {
		if (sems[i] == NULL)
			return i;
	}
	return MAX_SEMS;
}

sem_ret_t open_sem(semid_t sid, semvalue_t value)
{
	_acquire(&sems_lock);
	uint8_t sem_idx = get_idx(sid);
	if (sem_idx != MAX_SEMS) {
		_release(&sems_lock);
		return SEM_EXISTS;
	}

	sem_idx = get_free_idx();
	sems[sem_idx] = alloc(sizeof(sem_t));
	if (sems[sem_idx] == NULL) {
		_release(&sems_lock);
		return SEM_ENOMEM;
	}

	sems[sem_idx]->waiting_queue = queue_init();
	if (sems[sem_idx]->waiting_queue == NULL) {
		free(sems[sem_idx]);
		_release(&sems_lock);
		return SEM_ENOMEM;
	}

	sems[sem_idx]->name = sid;
	sems[sem_idx]->lock = 0;
	sems[sem_idx]->value = value;
	sems[sem_idx]->creator_process = get_curr_pid();

	_release(&sems_lock);
	return SEM_SUCCESS;
}

sem_ret_t wait_sem(semid_t sid)
{
	_acquire(&sems_lock);
	uint8_t sem_idx = get_idx(sid);
	if (sem_idx == MAX_SEMS) {
		_release(&sems_lock);
		return SEM_INVALID;
	}
	_acquire(&(sems[sem_idx]->lock));
	_release(&sems_lock);
	while (sems[sem_idx]->value <= 0) {
		set_process_state(get_curr_pid(), BLOCKED);
		enqueue(sems[sem_idx]->waiting_queue, get_curr_pid());
		_release(&(sems[sem_idx]->lock));
		yield_process();
		_acquire(&(sems[sem_idx]->lock));
	}

	sems[sem_idx]->value--;
	_release(&(sems[sem_idx]->lock));
	return SEM_SUCCESS;
}

sem_ret_t post_sem(semid_t sid)
{
	_acquire(&sems_lock);
	uint8_t sem_idx = get_idx(sid);
	if (sem_idx == MAX_SEMS) {
		_release(&sems_lock);
		return SEM_INVALID;
	}
	_acquire(&(sems[sem_idx]->lock));
	_release(&sems_lock);

	sems[sem_idx]->value++;
	queue_value_t wake_process;
	char success = peek(sems[sem_idx]->waiting_queue, &wake_process);
	if (success) {
		dequeue(sems[sem_idx]->waiting_queue);
		block(wake_process);
	}

	_release(&(sems[sem_idx]->lock));
	return SEM_SUCCESS;
}

sem_ret_t close_sem(semid_t sid)
{
	_acquire(&sems_lock);
	uint8_t sem_idx = get_idx(sid);
	if (sem_idx == MAX_SEMS) {
		_release(&sems_lock);
		return SEM_INVALID;
	}
	_acquire(&(sems[sem_idx]->lock));
	if (get_curr_pid() != sems[sem_idx]->creator_process || !is_empty(sems[sem_idx]->waiting_queue)) {
		_release(&(sems[sem_idx]->lock));
		_release(&sems_lock);
		return SEM_INVALID;
	}

	free(sems[sem_idx]);
	_release(&sems_lock);
	return SEM_SUCCESS;
}

void dump_sems(void)
{
	ncPrint("Semaphores\n");
	_acquire(&sems_lock);
	for (int i = 0; i < MAX_SEMS; i++) {
		if (sems[i] != NULL) {
			_acquire(&(sems[i]->lock));
			ncPrint(sems[i]->name);
			ncPrint(": ");
			ncPrintDec(sems[i]->value);
			ncPrint(", ");
			dump_queue(sems[i]->waiting_queue);
			_release(&(sems[i]->lock));
			ncNewline();
		}
	}
	ncNewline();
	_release(&sems_lock);
}
