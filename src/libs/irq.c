#include <irq.h>
#include <stdio.h>
#include <sched.h>
#include "core_timer_queue.h"

void irq_routing() {
    if(*CORE0_INTERRUPT_SOURCE & 0x2) {
        get_current()->resched = 1;
        core_timer_interrupt_handler();
    } else if (*CORE0_INTERRUPT_SOURCE & (1 << 8)) {

    }
    asm volatile("mrs x0, cntfrq_el0\n" "msr cntp_tval_el0, x0\n");
    /*printf("irq interrupt\n");*/
}
