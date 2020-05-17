#include "memory.h"
#include "sched.h"
#include "uart.h"
#include "fork.h"
#include "gpio.h"

extern struct task_struct *current;
extern struct task_struct * task_pool[NTASKS];
extern int tasks;

int task_create(int mode, void(*function)(), unsigned long stack){
	unsigned long func =(unsigned long) function;
	struct task_struct *p;

	p = (struct task_struct *) get_free_page();
	if (!p)
		return 1;

	struct pt_regs *childregs = task_pt_regs(p);
	memzero((unsigned long)childregs, sizeof(struct pt_regs));
	memzero((unsigned long)&p->context, sizeof(struct context));

	if (mode) {
		p->context.x19 = func;
		//p->context.x20 = arg;
	} else {
		struct pt_regs * cur_regs = task_pt_regs(current);
		*childregs = *cur_regs;
		childregs->regs[0] = 0;
		childregs->sp = stack + PAGE_SIZE;
		p->stack = stack;
	}

	p->mode = mode;
	p->state = RUNNING;
	//p->counter = p->priority;
	//p->preempt_flag = 1; //disable preemtion until schedule_tail
	p->reschedule = 0;
	//p->context.x19 = func;
	//p->cpu_context.x20 = arg;
	p->context.pc = (unsigned long)ret_from_fork;
	p->context.sp = (unsigned long)childregs;

	int curr_pid = current->taskid;
	int pid = tasks++;
	//char c =p->counter + '0';
	p->taskid = pid;
	task_pool[pid] = p;

	return pid;
}

int move_to_user_mode(unsigned long pc)
{
	struct pt_regs *regs = task_pt_regs(current);
	memzero((unsigned long)regs, sizeof(*regs));
	regs->pc = pc;
	regs->pstate = PSR_MODE_EL0t;
	unsigned long stack = get_free_page(); //allocate new user stack
	if (!stack) {
		return -1;
	}
	regs->sp = stack + PAGE_SIZE;
	current->stack = stack;
	return 0;
}

struct pt_regs * task_pt_regs(struct task_struct *tsk){
	unsigned long p = (unsigned long)tsk + THREAD_SIZE - sizeof(struct pt_regs);
	return (struct pt_regs *)p;
}
