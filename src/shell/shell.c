#include "shell.h"
#include "uart.h"
#include "cmd.h"
#include <string.h>

void parse(char* input, char arg[][100]){
    int flag = 0;
    int cmd_index = 0, arg_index = 0;
    for(int i = 0; input[i] != '\0'; i++){
        if(input[i] == ' ')
            flag = 1;

        if(flag == 0){
            arg[0][cmd_index ++] = input[i];
        }else{
            arg[1][arg_index ++] = input[i];
        }
    }
    arg[0][cmd_index] = '\0';
    arg[1][arg_index] = '\0';
}

void exec_cmd(char *input){
    char arg[2][100];
    parse(input, arg);
    if(!strcmp(arg[0], "help"))
        help();
    else if(!strcmp(arg[0], "hello"))
        hello(arg[1]);
    else if(!strcmp(arg[0], "reboot"))
        reboot();
    else 
        error_cmd();
} 

void shell(){
    while(1){
        char input[100];
        uart_get(input);
        
        exec_cmd(input);
    
        if(input[0] != '\n')
            uart_send('\n');
    }
}
