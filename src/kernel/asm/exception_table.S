.section ".text"

.macro save_all
    sub sp, sp, 32 * 8
    stp x0, x1, [sp ,16 * 0]
    stp x2, x3, [sp ,16 * 1]
    stp x4, x5, [sp ,16 * 2]
    stp x6, x7, [sp ,16 * 3]
    stp x8, x9, [sp ,16 * 4]
    stp x10, x11, [sp ,16 * 5]
    stp x12, x13, [sp ,16 * 6]
    stp x14, x15, [sp ,16 * 7]
    stp x16, x17, [sp ,16 * 8]
    stp x18, x19, [sp ,16 * 9]
    stp x20, x21, [sp ,16 * 10]
    stp x22, x23, [sp ,16 * 11]
    stp x24, x25, [sp ,16 * 12]
    stp x26, x27, [sp ,16 * 13]
    stp x28, x29, [sp ,16 * 14]
    str x30, [sp, 16 * 15]
.endm

.macro load_all
    ldp x0, x1, [sp ,16 * 0]
    ldp x2, x3, [sp ,16 * 1]
    ldp x4, x5, [sp ,16 * 2]
    ldp x6, x7, [sp ,16 * 3]
    ldp x8, x9, [sp ,16 * 4]
    ldp x10, x11, [sp ,16 * 5]
    ldp x12, x13, [sp ,16 * 6]
    ldp x14, x15, [sp ,16 * 7]
    ldp x16, x17, [sp ,16 * 8]
    ldp x18, x19, [sp ,16 * 9]
    ldp x20, x21, [sp ,16 * 10]
    ldp x22, x23, [sp ,16 * 11]
    ldp x24, x25, [sp ,16 * 12]
    ldp x26, x27, [sp ,16 * 13]
    ldp x28, x29, [sp ,16 * 14]
    ldr x30, [sp, 16 * 15]
    add sp, sp, 32 * 8
.endm

.align 11
.global exception_vector_table
exception_vector_table:
    b exception_handler 
    .align 7 
    b irq_handler
    .align 7
    b exception_handler
    .align 7
    b exception_handler
    .align 7

    b exception_handler
    .align 7
    b irq_handler
    .align 7
    b exception_handler
    .align 7
    b exception_handler
    .align 7

    b exception_handler
    .align 7
    b irq_handler
    .align 7
    b exception_handler
    .align 7
    b exception_handler
    .align 7

    b exception_handler
    .align 7
    b irq_handler
    .align 7
    b exception_handler
    .align 7
    b exception_handler
    .align 7


.global _set_exception_vector_table
_set_exception_vector_table:
    adr x0, exception_vector_table
    msr vbar_el1, x0
    ret

exception_handler:
    # enable interrupt
    msr DAIFSet, 0xf 
    save_all
    bl exception_level_info
    load_all
    # disable interrupt
    msr DAIFClr, 0xf
    eret

irq_handler:
    msr DAIFSet, 0xf
    save_all
    bl irq_routing
    load_all
    msr DAIFClr, 0xf
    eret

.global enable_interrupt
enable_interrupt:
    msr DAIFClr, 0xf
    ret

.global disable_interrupt
disable_interrupt:
    msr DAIFSet, 0xf
    ret
