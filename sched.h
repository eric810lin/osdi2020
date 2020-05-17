
#define NTASKS	64
#define THREAD_SIZE	4096
#define	RUNNING	1
#define TASK_ZOMBIE 0

#define kernel_mode		1
#define	user_mode 		0

extern struct	task_struct *current;
extern struct	task_struct * task_pool[NTASKS];
extern int	tasks;


struct context {
	unsigned long x19;
	unsigned long x20;
	unsigned long x21;
	unsigned long x22;
	unsigned long x23;
	unsigned long x24;
	unsigned long x25;
	unsigned long x26;
	unsigned long x27;
	unsigned long x28;
	unsigned long fp;
	unsigned long sp;
	unsigned long pc;
};

struct task_struct{
	struct 	context context;

	int mode;
	int	taskid;
	long	state;
	//long	priority;
	//long	counter;
	//long	preempt_flag;
	long	reschedule;
	unsigned long stack;
};

extern void schedule(void);
extern void context_switch(struct task_struct* next);
extern void cpu_switch_to(struct task_struct* prev, struct task_struct* next);
extern void ret_from_fork();
void exit_process(void);
int get_taskid(void);