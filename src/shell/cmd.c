#include "cmd.h"
#include "core_timer_queue.h"
#include <string.h>
#include <stdio.h>

void help(char argc[][100], int argv) {
    uart_put("help: print all available commands\n");
    uart_put("hello: print Hello World!\n");
    uart_put("loadimg\n");
    uart_put("ls\n");
    uart_put("cat\n");
    uart_put("svc: interrupt\n");
    uart_put("enable-timeout\n");
    uart_put("disable-timeout\n");
    uart_put("settimeout [message] [Timeout]\n");
    uart_put("reboot");
}

void hello(char argc[][100], int argv){
    uart_put("Hello World!");
    uart_put(argc[0]);
}

void reboot(char argc[][100], int argv){
    *PM_RSTC = (PM_PASSWORD | 0x20); // full reset
    *PM_WDOG = (PM_PASSWORD | 100);  // number of watchdog tick
}

void error_cmd(char argc[][100], int argv){
    uart_put("command not found");
}

void loadimg(char arg[][100], int argv){
    extern void *_bootloader_start;
    
    size_t load_address, img_size;
    /*size_t bootloader_size = (size_t)&_bootloader_end - (size_t)&_bootloader_start;*/
    char buf[9];

    /*read load address*/
    uart_put("input the address to load image(0x)");
    uart_nget(buf, 8);
    buf[8] = '\0';
    load_address = strtol(buf, 0, 16);
    uart_put("\nLoad image at: 0x");    
    uart_hex(load_address);

    /*read size*/
    uart_put("\ninput the image size(0x): ");
    uart_nget(buf, 8);
    buf[8] = '\0';
    uart_put(buf);
    img_size = strtol(buf, 0, 16);
    uart_put("\nimage size: ");
    uart_hex(img_size);
    uart_put("\n");
    uart_int(img_size);
    uart_put("\n");
    
    size_t relocate_img_jump = (size_t)&img_jump;

    if((load_address + img_size) > (size_t)&_bootloader_start) {
        char loss[img_size];
        uart_nget(loss, img_size);
        uart_put("load address will overlap bootloader \n");
        uart_put("please input other address \n");
        return ;
        /*size_t relocated_bootloader = load_address + img_size + 0x10000;*/
        /*relocated_bootloader = (relocated_bootloader >> 4) << 4;*/
        /*memcpy((char*) relocated_bootloader, &_bootloader_start, bootloader_size);*/
        /*relocate_img_jump = relocated_bootloader + ((size_t)&img_jump - (size_t)&_bootloader_start);*/
    }
    
    asm volatile("mov x0, %0\n" 
                 "mov x1, %1\n"
                 "mov sp, %2\n"
                 "blr %3 \n"::
                 "r"(load_address), 
                 "r"(img_size),
                 "r"(load_address),
                 "r"(relocate_img_jump):"x0", "x1");
}


void bootloader_get(char* buf, int n) {
    for(int i = 0; i < n; i++){
        buf[i] = uart_recv();
    }
}

void img_jump(size_t load_address, size_t img_size){
    //get image 
    bootloader_get((char*) load_address, img_size);
    
    asm volatile("mov sp, %0\n"
                 "blr %1\n"::
                 "r"(load_address),
                 "r"(load_address)); 
}

void nop(char argc[][100], int argv) { return; }

void ls(char argc[][100], int argv){
   get_all_filename(); 
}

void cat(char argc[][100], int argv){
    for(int i = 0; i < header_index; i++) {
        char buf[500];
        get_content(header[i].name_address, header[i].name_size, buf);
        if(!strcmp(argc[0], buf)){
            get_content(header[i].file_address, header[i].file_size, buf);
            uart_put(buf);
            return ;
        }
    }
    uart_put("can't find file ");
    uart_put(argc[0]);
}

void svc(char argc[][100], int argv) {
    asm volatile("svc #0");
    return; 
}

extern void core_timer_enable(void);
void enable_timeout(char argc[][100], int argv) {
    core_timer_enable();
    return ;
}

extern void core_timer_disable(void);
void disable_timeout(char argc[][100], int argv) {
    core_timer_disable();
    return ;
}

void settimeout(char argc[][100], int argv) {
    char mes[100];
    strncpy(mes, argc[0], 100);
    int timeout = string_to_int(argc[1]);
    printk("timeout: %d, mes: %s \n", timeout, mes);
    core_timer_queue_push(core_timer_print_message, timeout, mes, strlen(mes));
    return ;
}
