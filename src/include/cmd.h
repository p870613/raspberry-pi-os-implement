#ifndef _CMD_H
#define _CMD_H

#include <peripheral/uart.h>
#include <gpio.h>
#include <type.h>
#include <string.h>
#include <cpio.h>

void help(char[][100], int);
void hello(char[][100], int);
void reboot(char[][100], int);
void error_cmd(char[][100], int);
void nop(char[][100], int);
void loadimg(char[][100], int);
void ls(char[][100], int);
void cat(char[][100], int);
void svc(char[][100], int);
void enable_timeout(char[][100], int);
void disable_timeout(char[][100], int);

__attribute__((__section__(".bootloader"))) void bootloader_get(char*, int);
__attribute__((__section__(".bootloader"))) void img_jump(size_t, size_t);
#endif
