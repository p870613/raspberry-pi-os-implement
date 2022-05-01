#include "shell.h"
#include <string.h>

void parse(char* input, char* cmdline, char argc[][100], int* argv){
    int flag = 0, arg_flag = 0;
    int cmd_index = 0, arg_index = 0;
    for(int i = 0; input[i] != '\0'; i++){

        if(flag == 0 && input[i] == ' '){
            flag = 1;
            cmdline[cmd_index] = '\0';
        }else if(flag == 0){
            cmdline[cmd_index ++] = input[i];
        }else if(flag == 1 && input[i] == ' '){
            argc[*argv][arg_index] = '\0';
            (*argv) ++;
            arg_index = 0;
        }else if(flag == 1){
            argc[*argv][arg_index ++] = input[i];
            arg_flag = 1;
        }
    }
    if(arg_flag == 1)
        argc[(*argv)++][arg_index] = '\0';
    cmdline[cmd_index] = '\0';
        
}

void* cmd_state_machine(char* cmdline){
    if(!strcmp(cmdline, "help"))
        return &help;
    else if(!strcmp(cmdline, "hello"))
        return &hello;
    else if(!strcmp(cmdline, "reboot"))
        return &reboot;
    else if(!strcmp(cmdline, "loadimg"))
        return &loadimg;
    else if(!strcmp(cmdline, "ls"))
        return &ls;
    else if(!strcmp(cmdline, "cat"))
        return &cat;
    else if(!strcmp(cmdline, "svc"))
        return &svc;
    else if(!strcmp(cmdline, "enable-timeout"))
        return &enable_timeout;
    else if(!strcmp(cmdline, "disable-timeout"))
        return &disable_timeout;
    else if(!strcmp(cmdline, "\n") || !strcmp(cmdline, "") || !strcmp(cmdline, " "))
        return &nop;
    else   
        return &error_cmd;
}

void exec_cmd(char *input){
    char cmdline[100], argc[100][100];
    int argv = 0;
    parse(input, cmdline, argc, &argv);
    void (*fun_ptr)(char[][100], int) = cmd_state_machine(cmdline);
    fun_ptr(argc, argv);
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
