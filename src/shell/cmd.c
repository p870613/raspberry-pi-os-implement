#include "cmd.h"

void help(void){
    uart_put("help: print all available commands\n");
    uart_put("hello: print Hello World!\n");
    uart_put("reboot");
}

void hello(char* arg){
    uart_put("Hello World!");
    uart_put(arg);
}

void reboot(void){
    
}

void error_cmd(void){
    uart_put("command not found");
}
