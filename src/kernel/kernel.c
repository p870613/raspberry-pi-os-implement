#include "uart.h"
#include "shell.h"

void kernel_main(void){
    uart_init();
    uart_put("hello world \r\n");
    
    shell();
    /*while (1){*/
        /*char input[100];*/
        /*uart_get(input);*/
        /*uart_put(input);*/
    /*}*/
} 

