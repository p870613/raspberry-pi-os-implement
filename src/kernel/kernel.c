#include <peripheral/uart.h>
#include <peripheral/mailbox.h>
#include <shell.h>
#include <cpio.h>
#include <mm/buddy.h>
#include <mm/dynamic.h>
#include <stdio.h>
#include <stdlib.h>
#include <core_timer_queue.h>
#include <exception.h>
#include <sched.h>
#include <timer.h>

void kernel_main(void){
    uart_init();
    uart_put("********** hello world ***********\n");
    cpio_parse(CPIO_ADDRESS);
    core_timer_queue_init();
    buddy_init();
    dynamic_init();
    exception_level_info();
    core_timer_enable();
    task_user();
    shell();
} 
