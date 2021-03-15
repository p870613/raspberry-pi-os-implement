#ifndef _CMD_H
#define _CMD_H

#include <peripheral/uart.h>
#include <gpio.h>

void help(char*);
void hello(char*);
void reboot(char*);
void error_cmd(char*);
void nop(char*);
#endif
