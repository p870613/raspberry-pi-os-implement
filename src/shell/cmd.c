#include "cmd.h"
#include <uart.h>
#include <gpio.h>
void help(char* arg) {
    uart_put("help: print all available commands\n");
    uart_put("hello: print Hello World!\n");
    uart_put("reboot");
}

void hello(char* arg){
    uart_put("Hello World!");
    uart_put(arg);
}

void reboot(char *arg){
    *PM_RSTC = (PM_PASSWORD | 0x20); // full reset
    *PM_WDOG = (PM_PASSWORD | 100); // number of watchdog tick
}

void error_cmd(char* arg){
    uart_put("command not found");
}

void nop(char* arg) {return;}
