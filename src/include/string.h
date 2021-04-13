#ifndef _STRING_H
#define _STRING_H

#include <type.h>
#include <peripheral/uart.h>
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* dest, uint8_t val, size_t n);

size_t strlen(const char* s);
int strcmp(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, size_t n);
char* strcat(char* dest, const char* src);
long strtol(const char*, const char **__restrict, int);

#define sizeof(x) ((char*)(&x + 1) - (char*)(&x))
#endif
