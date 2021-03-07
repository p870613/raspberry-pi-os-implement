#include "uart.h"

void kernel_main(void){
    uart_init();
    uart_write("hello world \r\n");

    while (true)
        uart_read(uart_recv());
} 

