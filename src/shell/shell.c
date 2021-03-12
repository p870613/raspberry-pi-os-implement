#include "shell.h"
#include "uart.h"
#include "cmd.h"
#include <string.h>

void parse(char* input, char* cmdline, char* arg){
    int flag = 0;
    int cmd_index = 0, arg_index = 0;
    for(int i = 0; input[i] != '\0'; i++){
        if(input[i] == ' ')
            flag = 1;

        if(flag == 0){
            cmdline[cmd_index ++] = input[i];
        }else{
            arg[arg_index ++] = input[i];
        }
    }
    cmdline[cmd_index] = '\0';
    arg[arg_index] = '\0';
}

void* cmd_state_machine(char* cmdline){
    if(!strcmp(cmdline, "help"))
        return &help;
    else if(!strcmp(cmdline, "hello"))
        return &hello;
    else if(!strcmp(cmdline, "reboot"))
        return &reboot;
    else if(!strcmp(cmdline, "\n") || !strcmp(cmdline, "") || !strcmp(cmdline, " "))
        return &nop;
    else   
        return &error_cmd;
}

void exec_cmd(char *input){
    char cmdline[100], arg[100];
    parse(input, cmdline, arg);
    void (*fun_ptr)(char*) = cmd_state_machine(cmdline);
    fun_ptr(arg);
} 

void shell(){
    while(1){
        char input[100];
        uart_put("% ");
        uart_get_cmd(input);
        
        exec_cmd(input);
    
        if(strcmp(input, "")){
            uart_put("\n");
        }
    }
}
