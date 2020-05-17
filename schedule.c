#include "sched.h"
#include "uart.h"
#include "irq_h.h"
#include "memory.h"

static struct task_struct init_task = { {0,0,0,0,0,0,0,0,0,0,0,0,0}, kernel_mode, 0,1,1,1};
struct task_struct *current = &(init_task);
struct task_struct * task_pool[NTASKS] = {&(init_task), };
int tasks = 1;

int get_taskid(void)
{
	return current->taskid; 
}

void context_switch(struct task_struct * next) 
{
		if (current == next) 
			return;
		struct task_struct * prev = current;
		current = next;
		cpu_switch_to(prev, next);
}

void schedule(void)
{
	int next;
	struct task_struct * p;
	while (current->reschedule) {
		//c = -1;
		next = 0;
		for (int i = 0; i < NTASKS; i++){
			p = task_pool[i];
			if (p && p->state == RUNNING && p != current) {
				//c = p->counter;
				next = i;
			}
		}
		if (next) {
			current->reschedule = 0;
			context_switch(task_pool[next]);
			break;
		}/*
		for (int i = 0; i < NTASKS; i++) {
			p = task_pool[i];
			if (p) {
				p->counter = (p->counter >> 1) + p->priority;
			}
		}*/
	}
}

void core_timer_handler()
{
	current->reschedule = 1;
	asm volatile("mov x0, 0xf0000\n\t"
		      "msr cntp_tval_el0, x0\n\t"
		     );
	return;
}

void exit_process(){
	current->reschedule = 0;
	for (int i = 0; i < NTASKS; i++){
		if (task_pool[i] == current) {
			task_pool[i]->state = TASK_ZOMBIE;
			break;
		}
	}
	if (current->stack) {
		free_page(current->stack);
	}
	current->reschedule = 1;
	schedule();
}
/*
void timer_tick()
{
	char c = current->counter+'0';
	uart_send(c);
	uart_send('\n');
	
	--current->counter;
	if (current->counter>0) {
	uart_send('i');
		return;
	}
	uart_send('\n');
	current->counter=0;
	irq_disable();
	uart_send('\n');
	schedule();
	irq_enable();
}*/
