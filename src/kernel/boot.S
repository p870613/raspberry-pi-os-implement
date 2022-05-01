#include <mm/mm.h>

.section ".text"

.global _start

_start:
    mrs x0, mpidr_el1 // get cpu information
    and x0, x0, #0xFF //check processer id
    cbz x0, _kernel_loader 
    b hang //hang for non-primary CPU

hang:
    wfe
    b hang

_kernel_loader: 
    ldr x0, =_start
    mov sp, x0
    ldr x0, =_bss_start
    ldr x1, =_bss_end
    sub x1, x1, x0
    bl memzero //clear _bss

_from_el2_to_el1:
    # EL1 use aarch64
    mov x0, #(1<<31)
    msr hcr_el2, x0
    
    # set el1 stack pointer
    ldr x0, =_start
    msr sp_el1, x0
    
    bl _set_exception_vector_table
    mov x0, #0x3c5
    msr  spsr_el2, x0
    
    # set return address
    ldr  x0, =_from_el1_to_el0
    msr  elr_el2, x0
    eret

_from_el1_to_el0:
    # set el0 stack pointer 
    ldr x0, =_start
    sub x0, x0, #0x1000
    msr sp_el0, x0
    
    # set state to el1
    mov x0, #0x3c0
    msr  spsr_el1, x0

    # disable exception fro, SIMD
    mrs x0, cpacr_el1
    orr x0, x0, #(3 << 20)
    msr cpacr_el1, x0
   
    # return to el0
    # ldr x0, =_load_main
    # msr elr_el1, x0
 
    # enable interrupt
    msr DAIFClr, 0xf
    #bl core_timer_enable
    #eret

_load_main:
    bl kernel_main 
