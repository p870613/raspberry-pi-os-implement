#include "mm.h"

.section ".text"

.global _start

_start:
    mrs x0, mpidr_el1 // get cpu information
    and x0, x0, #0xFF //check processer id
    cbz x0, master 
    b hang //hang for non-primary CPU

hang:
    b hang

master:
    adr x0, _bss_start
    adr x1, _bss_end
    sub x1, x1, x0
    bl memzero

    mov sp, #LOW_MEMORY
    bl kernel_main 

     


    
