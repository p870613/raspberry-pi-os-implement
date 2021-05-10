#include <peripheral/uart.h>
#include <peripheral/mailbox.h>
#include <shell.h>
#include <cpio.h>
#include <mm/buddy.h>

void kernel_main(void){
    uart_init();
    uart_put("********** hello world ***********\n");
    cpio_parse(CPIO_ADDRESS);
    buddy_init();
    /*uart_int(header_index);*/
    /*print_free_list();*/
    /*char* a = (char*)buddy_allocate(4098);*/
    /*print_free_list();*/
    /*int* b = (int*)buddy_allocate(20);*/
    /*print_free_list();*/
    /*buddy_free(b);*/
    /*print_free_list();*/
    /*buddy_free(a);*/
    /*print_free_list();*/

    shell();
} 

