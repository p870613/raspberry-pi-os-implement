#include "peripheral/uart.h"
#include "peripheral/mailbox.h"
#include "shell.h"


void kernel_main(void){
    uart_init();
    uart_put("********** hello world ***********\n");

    shell();
} 

