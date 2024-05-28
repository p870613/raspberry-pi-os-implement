#include <irq.h>
#include <stdio.h>
#include <sched.h>
#include "core_timer_queue.h"

void irq_routing() {
    if(*CORE0_INTERRUPT_SOURCE & 0x2) {
        get_current()->resched = 1;
        core_timer_interrupt_handler();
        schedule();
    } else if (*CORE0_INTERRUPT_SOURCE & (1 << 8)) {

    }
    /*printf("irq interrupt\n");*/
}
