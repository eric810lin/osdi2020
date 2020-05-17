#include "uart.h"
#include "time.h"
#include "irq_h.h"

const char *entry_error_messages[] = {
	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",		
	"FIQ_INVALID_EL1t",		
	"ERROR_INVALID_EL1T",		

	"SYNC_INVALID_EL1h",		
	"IRQ_INVALID_EL1h",		
	"FIQ_INVALID_EL1h",		
	"ERROR_INVALID_EL1h",		

	"SYNC_INVALID_EL0_64",		
	"IRQ_INVALID_EL0_64",		
	"FIQ_INVALID_EL0_64",		
	"ERROR_INVALID_EL0_64",	

	"SYNC_INVALID_EL0_32",		
	"IRQ_INVALID_EL0_32",		
	"FIQ_INVALID_EL0_32",		
	"ERROR_INVALID_EL0_32"	
};

void show_invalid_entry_message(int type,unsigned long esr, unsigned long address)
{
	char *c = entry_error_messages[type];
	//uart_puts(entry_error_messages[type]);
	uart_puts(c);
	uart_puts(", ESR: ");
	uart_hex(esr);	
	uart_puts(", address: ");
	uart_hex(address);	
	uart_puts("\r\n");
}

void exception_handler(void)
{
  unsigned long esr=0,elr=0,el;
  unsigned int  ec,iss;
  asm volatile ("mrs %0, CurrentEL" : "=r" (el));
  el = (el>>2)&3;
  if(el == 2){
  	asm volatile ("mrs %0, elr_el2\n\t"
			"mrs %1, esr_el2\n\t"
			:"=r"(elr),"=r"(esr)
		     );
  }
  else if(el == 1){
  	asm volatile ("mrs %0, elr_el1\n\t"
			"mrs %1, esr_el1\n\t"
			:"=r"(elr),"=r"(esr)
		     );
  
  }
  ec = esr>>26;
  iss= esr&0xffffff;
  if(iss){	  
      uart_puts(" Exception return address 0x");
      uart_hex(elr);
      uart_puts("\n");

      uart_puts(" Exception class (EC) 0x");
      uart_hex(ec);
      uart_puts("\n");
  
      uart_puts(" Instruction specific syndrome (ISS) 0x");
      uart_hex(iss);
      uart_puts("\n");
  }
  else{
      irq_enable();
      core_timer_enable();
      while(1);
  }
}

void irq_vector_init(){
	asm volatile ("adr     x0, vectors\n\t"
		      "msr     vbar_el1,x0\n\t"
		      "ret\n\t"
		      );
}

void irq_enable(){
	asm volatile ("msr     daif, xzr\n\t"
		      "ret\n\t"
		      );
}

void irq_disable(){
	asm volatile ("msr     daifset,#2\n\t"
		      "ret\n\t"
		     );
}

void irq_handler(void)
{
  	//uart_puts(" core timer interrupt\n");
	core_timer_handler();
}
