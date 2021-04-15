#include "cmd.h"

void help(char* arg) {
    uart_put("help: print all available commands\n");
    uart_put("hello: print Hello World!\n");
    uart_put("loadimg\n");
    uart_put("reboot");
}

void hello(char* arg){
    uart_put("Hello World!");
    uart_put(arg);
}

void reboot(char *arg){
    *PM_RSTC = (PM_PASSWORD | 0x20); // full reset
    *PM_WDOG = (PM_PASSWORD | 100);  // number of watchdog tick
}

void error_cmd(char* arg){
    uart_put("command not found");
}

void loadimg(char* arg){
    extern void *_bootloader_end, *_bootloader_start;
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

void nop(char* arg) { return; }
