#include "uart.h"
#include "shell.h"

void kernel_main(void){
    uart_init();
    uart_put("********** hello world ***********\r\n");
    
    shell();
} 

