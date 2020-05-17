#include "sysregset.h"

.section ".text.boot"

.global _start
.global gtime_frq
.global gtime_ct
  

_start:
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f


    // cpu id > 0, stop
1:  wfe
    b       1b

2:  // cpu id == 0
    // set stack before our code
    ldr     x1, =_start
    mov     sp, x1
    // set up EL1
    mrs     x0, CurrentEL
    and     x0, x0, #12 // clear reserved bits

    // running at EL3?
    cmp     x0, #12
    bne     5f
    // should never be executed, just for completeness
    ldr     x2, =SCR_set
    msr     scr_el3, x2

    ldr     x2, =SPSR_set
    msr     spsr_el3, x2

    adr     x2, 5f
    msr     elr_el3, x2
    eret

    // running at EL2?
5:  cmp     x0, #8
    bne     6f
/*
    // set up exception handlers
    ldr     x2, =_vectors
    msr     vbar_el1, x2
    msr     vbar_el2, x2
*/

    //setup EL1 stack
    adr     x2,_start
    msr     sp_el1, x2
    
//enable interrupt
    // enable AArch64 in EL1
    ldr     x2, =HCR_set
    msr     hcr_el2, x2

    ldr     x2, =SCTLR_set
    msr     sctlr_el1, x2
    
    ldr     x2, =TCR_set
    msr     tcr_el1, x2

    ldr     x2, =MAIR_set
    msr     mair_el1, x2
    // switch to EL1
    mov x2, 0x5
    msr spsr_el2, x2
    adr x2, 7f
    msr elr_el2, x2
    eret

6:  /*mrs     x0, CurrentEL
    and     x0, x0, #12 // clear reserved bits
    cmp     x0, #4
    bne     7f
    //set EL0 stack
    adr     x0, _start
    sub     x0, x0, #0x10000
    msr     sp_el0, x0
    //switch to EL0
    mov     x0,0x0
    msr     spsr_el1, x0
    adr     x0, 7f
    msr     elr_el1, x0
    eret*/
7:  


    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
3:  cbz     w2, 4f
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, 3b    
	
4:  bl      main
    b       1b




/*

synchronous_EL1:
	kernel_entry 
	bl	exception_handler
	kernel_exit 

irq_EL2:
	kernel_entry 
	bl	irq_handler
	kernel_exit 

no_implement:
	kernel_entry
	kernel_exit

gtime_frq:
	mrs x0, cntfrq_el0
	ret 
gtime_ct:
	mrs x0, cntpct_el0
	ret 
*/
