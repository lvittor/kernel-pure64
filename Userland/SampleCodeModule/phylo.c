// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <phylo.h>
#include <string.h>
#include <utils.h>
#include <lib.h>

#define SEM_PREFIX "CHOPSTICK_"
#define SEM_PHILO_ID_LEN 13

static philo_state_t philos_state[MAX_PHILOSOPHERS];
static pid_t philos_pid[MAX_PHILOSOPHERS];
static uint8_t philos_count = MIN_PHILOSOPHERS;
static semid_t chopsticks[MAX_PHILOSOPHERS], table = "table", mutex = "mutex";

static void philosopher(int argc, char *argv[]);
static void eat(int philo_id);
static void pickChopsticks(int philo_id);
static void returnChopsticks(int philo_id);
static void printPhilos();
static int getLeftChopstick(int philo_id);
static int getRightChopstick(int philo_id);
static void closeSems(int amount);
static void killPhilos(int amount);
static void philosophing(int philo_id);

void initPhilos(void)
{
	print_f(STDOUT, "Agregar: [%c] Remover: [%c] Cerrar: [%c] \n",
		ADD_PHILOSOPHER_CHAR, REMOVE_PHILOSOPHER_CHAR,
		QUIT_PHILOSOPHER_CHAR);

	while (1) {
		for (int i = 0; i < philos_count; i++)
			philos_state[i] = PHILOSOPHING;

		char philo_id[2];
		philo_id[1] = '\0';

		if (_open_sem(table, philos_count - 1) != SEM_SUCCESS) {
			_exit();
		}

		if (_open_sem(mutex, 1) != SEM_SUCCESS) {
			_close_sem(table);
			_exit();
		}

		for (int i = 0; i < philos_count; i++) {
			char sem_chopstick_id[SEM_PHILO_ID_LEN] = SEM_PREFIX;
			philo_id[0] = i + '0';
			strcat(sem_chopstick_id, philo_id);
			if (_open_sem(sem_chopstick_id, 1) != SEM_SUCCESS) {
				killPhilos(i);
				closeSems(i);
				_close_sem(table);
				_close_sem(mutex);
				_exit();
			}

			chopsticks[i] = _alloc(strlen(sem_chopstick_id) + 1);
			strcopy(chopsticks[i], sem_chopstick_id);

			_process_prototype_t philoStruct = {
				.function_address = (void *)philosopher,
				.name = "PHILO",
				.priority = 0,
				.state = READY,
				.fds = { STDIN, STDOUT, STDERR },
				.foreground = 0,
			};

			char *argv[] = { philo_id };
			philos_pid[i] = _create_process(&philoStruct, 1, argv);
		}

		int c;
		if ((c = _get_char(STDIN)) < 0) {
			killPhilos(philos_count);
			closeSems(philos_count);
			_close_sem(table);
			_close_sem(mutex);
			_exit();
		}

		killPhilos(philos_count);
		closeSems(philos_count);
		_close_sem(table);
		_close_sem(mutex);

		if (c == QUIT_PHILOSOPHER_CHAR) {
			break;
		} else if (c == ADD_PHILOSOPHER_CHAR) {
			if (philos_count >= MAX_PHILOSOPHERS)
				print_f(STDERR,
					"Ya hay la maxima cantidad de filosofos\n");
			else
				philos_count++;
		} else if (c == REMOVE_PHILOSOPHER_CHAR) {
			if (philos_count <= MIN_PHILOSOPHERS)
				print_f(STDERR,
					"Ya hay la minima cantidad de filosofos\n");
			else
				philos_count--;
		}
	}
	_exit();
}

static void philosopher(int argc, char *argv[])
{
	int philo_id = atoi(argv[0]);
	while (1) {
		philosophing(philo_id);
		_wait_sem(table);
		eat(philo_id);
		_post_sem(table);
	}
	_exit();
}

static void philosophing(int philo_id)
{
	sleep((philo_id + 2) % 3 + 1);
}

static void eat(int philo_id)
{
	pickChopsticks(philo_id);

	_wait_sem(mutex);
	philos_state[philo_id] = EATING;
	printPhilos();
	_post_sem(mutex);

	sleep((philo_id + 3) % 2 + 1);

	_wait_sem(mutex);
	philos_state[philo_id] = PHILOSOPHING;
	_post_sem(mutex);

	returnChopsticks(philo_id);
}

static void pickChopsticks(int philo_id)
{
	_wait_sem(mutex);
	philos_state[philo_id] = HUNGRY;
	_post_sem(mutex);

	_wait_sem(chopsticks[getRightChopstick(philo_id)]);
	_wait_sem(chopsticks[getLeftChopstick(philo_id)]);
}

static void returnChopsticks(int philo_id)
{
	_post_sem(chopsticks[getLeftChopstick(philo_id)]);
	_post_sem(chopsticks[getRightChopstick(philo_id)]);
}

static void printPhilos()
{
	for (int i = 0; i < philos_count; i++)
		if (philos_state[i] == EATING)
			print_f(STDOUT, "%s", EATING_STR);
		else if (philos_state[i] == HUNGRY)
			print_f(STDOUT, "%s", HUNGRY_STR);
		else
			print_f(STDOUT, "%s", PHILOSOPHING_STR);
	print_f(STDOUT, "\n");
}

static int getLeftChopstick(int philo_id)
{
	return (philo_id + philos_count - 1) % philos_count;
}

static int getRightChopstick(int philo_id)
{
	return philo_id;
}

static void closeSems(int amount)
{
	for (int i = 0; i < amount; i++) {
		_close_sem(chopsticks[i]);
		_free(chopsticks[i]);
	}
}

static void killPhilos(int amount)
{
	for (int i = 0; i < amount; i++) {
		_kill(philos_pid[i]);
	}
}