#include <stdio.h>
#include <test_util.h>
#include <lib.h>

//TO BE INCLUDED
void endless_loop(){
  while(1);
}

uint32_t my_create_process(char * name){
  _process_prototype_t process = {
			.function_address = (void *)endless_loop,
			.name = name,
			.priority = 0,
			.state = READY,
			.fds = { STDIN, STDOUT, STDERR },
			.foreground = 0,
		};
  return  _create_process(&process, 0, (char *[]){ NULL });
}

uint32_t my_kill(uint32_t pid){
  return _kill(pid);
}

uint32_t my_block(uint32_t pid){
  return _block(pid);
}

uint32_t my_unblock(uint32_t pid){
  return _block(pid);
}

#define MAX_PROCESSES 102 //Should be around 80% of the the processes handled by the kernel

typedef struct P_rq{
  uint32_t pid;
  process_state_t state;
}p_rq;

void test_processes(void){
  p_rq p_rqs[MAX_PROCESSES];
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;

  print_f(STDOUT, "Comenzando la prueba...\n");

  while (1){
    // Create MAX_PROCESSES processes
    for(rq = 0; rq < MAX_PROCESSES; rq++){
      p_rqs[rq].pid = my_create_process("endless_loop");  // TODO: Port this call as required

      if (p_rqs[rq].pid == -1){                           // TODO: Port this as required
        print_f(STDERR, "Error creating process\n");
        return;
      }else{
        p_rqs[rq].state = READY;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0){

      for(rq = 0; rq < MAX_PROCESSES; rq++){
        action = GetUniform(2) % 2; 

        switch(action){
          case 0:
            if (p_rqs[rq].state == READY || p_rqs[rq].state == BLOCKED){
              if (my_kill(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                print_f(STDERR, "Error killing process\n");
                return;
              }
              p_rqs[rq].state = KILLED; 
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == READY){
              if(my_block(p_rqs[rq].pid) == -1){          // TODO: Port this as required
                print_f(STDERR, "Error blocking process\n");
                return;
              }
              p_rqs[rq].state = BLOCKED; 
            }
            break;
        }
      }

      // Randomly unblocks processes
      for(rq = 0; rq < MAX_PROCESSES; rq++)
        if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
          if(my_unblock(p_rqs[rq].pid) == -1){            // TODO: Port this as required
            print_f(STDERR, "Error unblocking process\n");
            return;
          }
          p_rqs[rq].state = READY; 
        }
    }
    print_f(STDOUT, ".");
  }
}