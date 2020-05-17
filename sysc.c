#include "uart.h"
#include "fork.h"
#include "memory.h"
#include "sched.h"


void sys_write(char * buf){
	uart_puts(buf);
}

char sys_read(){
	char r;
	r = uart_getc();
	return r;
}

int sys_clone(unsigned long stack){
	return task_create(0, 0, stack);
}

unsigned long sys_malloc(){
	unsigned long addr = get_free_page();
	if (!addr) {
		return -1;
	}
	return addr;
}

void sys_exit(){
	exit_process();
}

void * const sys_call_table[] = {sys_write, sys_read, sys_clone, sys_exit, sys_malloc};
