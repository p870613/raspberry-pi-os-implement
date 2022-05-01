#include <irq.h>
#include <stdio.h>

void irq_routing() {
    asm volatile("mrs x0, cntfrq_el0\n" "msr cntp_tval_el0, x0\n");
    printf("irq interrupt\n");
}
