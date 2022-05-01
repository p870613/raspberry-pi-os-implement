#include <peripheral/uart.h>
#include <peripheral/mailbox.h>
#include <shell.h>
#include <cpio.h>
#include <mm/buddy.h>
#include <mm/dynamic.h>
#include <stdio.h>
void kernel_main(void){
    uart_init();
    uart_put("********** hello world ***********\n");
    cpio_parse(CPIO_ADDRESS);
    buddy_init();
    dynamic_init();
    exception_level_info();
    shell();
} 

