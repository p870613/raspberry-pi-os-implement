#ifndef _CMD_H
#define _CMD_H

#include <peripheral/uart.h>
#include <gpio.h>
#include <type.h>
#include <string.h>
void help(char*);
void hello(char*);
void reboot(char*);
void error_cmd(char*);
void nop(char*);
void loadimg(char*);

__attribute__((__section__(".bootloader"))) void img_jump(size_t, size_t);

#endif
