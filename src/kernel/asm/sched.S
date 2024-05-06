.section ".text"

.global switch_to
switch_to:
    stp x19, x20, [x0, 0x10 * 0]
    stp x21, x22, [x0, 0x10 * 1]
    stp x23, x24, [x0, 0x10 * 2]
    stp x25, x26, [x0, 0x10 * 3]
    stp x27, x28, [x0, 0x10 * 4]
    stp fp, lr, [x0, 0x10 * 5]
    mov x9, sp
    str x9, [x0, 0x10 * 6]

    ldp x19, x20, [x1, 0x10 * 0]
    ldp x21, x22, [x1, 0x10 * 1]
    ldp x23, x24, [x1, 0x10 * 2]
    ldp x25, x26, [x1, 0x10 * 3]
    ldp x27, x28, [x1, 0x10 * 4]
    ldp fp, lr, [x1, 0x10 * 5]
    ldr x9, [x1, 0x10 * 6]
    mov sp, x9
    msr tpidr_el1, x1
    ret

.global get_current
get_current:
    mrs x0, tpidr_el1
    ret     
