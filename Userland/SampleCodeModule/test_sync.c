#include <stdint.h>
#include <stdio.h>
#include <lib.h>

static void inc(int argc, char * argv[]);

static uint32_t my_create_process(char *name, int argc, char * argv[])
{
	_process_prototype_t process = {
		.function_address = (void *)inc,
		.name = name,
		.priority = 0,
		.state = READY,
		.fds = { STDIN, STDOUT, STDERR },
		.foreground = 0,
	};
	return _create_process(&process, argc, argv);
}

static uint64_t my_sem_open(char *sem_id, uint64_t initialValue){
  return _open_sem(sem_id, initialValue);
}

static uint64_t my_sem_wait(char *sem_id){
  return _wait_sem(sem_id);
}

static uint64_t my_sem_post(char *sem_id){
  return _post_sem(sem_id);
}

static uint64_t my_sem_close(char *sem_id){
  return _close_sem(sem_id);
}

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int64_t global = 0;  //shared memory

static void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  _yield();
  *p = aux;
}

static void inc(int argc, char * argv[]) {
  uint64_t sem;
  int64_t value;
  uint64_t N;
  sscan(argv[0], "%d", &sem);
  sscan(argv[1], "%d", &value);
  sscan(argv[2], "%d", &N);

  print_f(STDOUT, "{%d}{%d}{%d}\n", sem, value, N);

  uint64_t i;
  sem_ret_t ans = my_sem_open(SEM_ID, 1);
  if (sem && ans != SEM_SUCCESS && ans != SEM_EXISTS){
    print_f(STDERR, "ERROR OPENING SEM\n");
    return;
  }
  
  print_f(STDOUT, "Por sumar\n", sem, value, N);
  
  for (i = 0; i < N; i++){
    if (sem) my_sem_wait(SEM_ID);
    slowInc(&global, value);
    print_f(STDOUT, ",");
    if (sem) my_sem_post(SEM_ID);
  }
  print_f(STDOUT, "Ya sume\n");

  if (sem) my_sem_close(SEM_ID);
  
  print_f(STDOUT, "Final value: %d\n", global);
  _dump_pipes();
  _exit();
}

void test_sync(void){
  uint64_t i;

  global = 0;

  print_f(STDOUT, "CREATING PROCESSES...(WITH SEM)\n");

  char * argvSum[] = {
    "1",
    "1",
    "100",
    NULL,
  };
  char * argvMinus[] = {
    "1",
    "-1",
    "100",
    NULL,
  };

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    print_f(STDERR, "pid:%d\n", my_create_process("inc", 3, argvSum));
    print_f(STDERR, "pid:%d\n", my_create_process("inc", 3, argvMinus));
  }

  _exit();
}

void test_no_sync(void){
  uint64_t i;

  global = 0;

  print_f(STDOUT, "CREATING PROCESSES...(WITHOUT SEM)\n");

  char * argvSum[] = {
    "0",
    "1",
    "100",
    NULL,
  };
  char * argvMinus[] = {
    "0",
    "-1",
    "100",
    NULL,
  };

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process("inc", 3, argvSum);
    my_create_process("inc", 3, argvMinus);
  }

  _exit();
}