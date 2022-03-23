#include <peripheral/uart.h>
#include <peripheral/mailbox.h>
#include <shell.h>
#include <cpio.h>
#include <mm/buddy.h>
#include <mm/dynamic.h>

void kernel_main(void){
    uart_init();
    uart_put("********** hello world ***********\n");
    cpio_parse(CPIO_ADDRESS);
    buddy_init();
    dynamic_init();
    dynamic_status();
    void* a = dynamic_malloc(0x100);
    void* b = dynamic_malloc(0x100);
    void* c = dynamic_malloc(0x100);
    void* d = dynamic_malloc(0x100);
    dynamic_status();
    dynamic_free(a);
    dynamic_status();
    dynamic_free(b);
    dynamic_status();
    dynamic_free(c);
    dynamic_status();
    dynamic_free(d);
    dynamic_status();
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

