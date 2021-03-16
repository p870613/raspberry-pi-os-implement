#include "peripheral/uart.h"
#include "peripheral/mailbox.h"
#include "shell.h"


void kernel_main(void){
    uart_init();
    uart_put("********** hello world ***********\n");
    uart_put("Board Revision: ");

    uart_hex(get_board_revision());
    uart_put("\n");

    shell();
} 

