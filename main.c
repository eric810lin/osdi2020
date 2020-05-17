#include "uart.h"
#include "mailbox.h"
#include "time.h"
#include "irq_h.h"
#include "sched.h"
#include "fork.h"
#include "sysc.h"

#define input_buffer_Max 64


extern int gtime_frq(void);
extern int gtime_ct(void);

/*static struct task_struct init_task = { {0,0,0,0,0,0,0,0,0,0,0,0,0}, 0,0,1};
struct task_struct *current = &(init_task);
struct task_struct * task_pool[NTASKS] = {&(init_task), };
int tasks = 1;*/
/*int strcmp(char *a, char *b)
{
	while(*a != '\0' || *b != '\n'){
		if(*a != *b) return -1;
		a++;
		b++;
	}
	return 0;
}*/
void foo(){
	int tmp = 5;
	int pid = get_taskid();
	//char r2;
	//r2 = tmp - '0';
	call_sys_write("Task ");
	uart_hex(pid);
	call_sys_write(" after exec, tmp address 0x");
	uart_hex((unsigned long)&tmp);
	call_sys_write(", tmp value ");
	uart_hex((unsigned long)tmp);
	uart_send('\n');
  	//printf("Task %d after exec, tmp address 0x%x, tmp value %d\n", get_taskid(), &tmp, tmp);
  	call_sys_exit();
	//while(1){
  	call_sys_write("sye_exit fail!\n");

	//	uart_puts("foo\n");
	//	schedule();
	//}
}
void test(){
	int cnt =1;
	unsigned long stack = call_sys_malloc();

	if(call_sys_clone((unsigned long)&foo, 0, stack) == 0)
	{
		call_sys_write("sye_exit fail!\n");
		stack = call_sys_malloc();
		call_sys_clone((unsigned long)&foo, 0, stack);

		unsigned long stack = call_sys_malloc();
		call_sys_clone(0, 0, stack);

		while(cnt < 10) {
			call_sys_write("Task id:");
			uart_hex(get_taskid());
			call_sys_write(", cnt: ");
			uart_hex(cnt);
			call_sys_write("\n");
      		//printf("Task id: %d, cnt: %d\n", get_taskid(), cnt);
      		//delay(100000);
      		++cnt;
    	}
    	call_sys_exit();
	}
	else{
				call_sys_write("foo\n");

	}
	
}

void user_test(){
    uart_puts("Kernel process started.\n");
    int err = move_to_user_mode((unsigned long)&test);
    if (err < 0){
        uart_puts("Error while moving process to user mode\n\r");
    }
}

void idle(){
  int i=1000000000;
  while(1){
    if(tasks == 1) {
      break;
    }
    schedule();
    while(i--){};
  }
  uart_puts("Test finished\n");
  while(1);
}

void require1(){
    while (1){
        uart_puts("require1.\n");
        schedule();
    }
}
void require2(){
    while (1){
    	uart_puts("require2require2.\n");
        schedule();
    }
}

void main()
{
	//char s[input_buffer_Max];
	//int i=0,j;
	unsigned long el;

	uart_init();
	irq_vector_init();
	irq_enable();
	core_timer_enable();

	asm volatile ("mrs %0, CurrentEL" : "=r" (el));

	uart_puts("Current EL is: ");
	uart_hex((el>>2)&3);
	uart_puts("\n");
	
	/*require1.2*/
	task_create(kernel_mode, require1, 0);
	task_create(kernel_mode, require2, 0);

	/*require3
	task_create(kernel_mode, user_test, 0);
	idle();*/
	while(1){
		schedule();
	}
}

