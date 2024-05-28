#include <syscall.h>
#include <sched.h>
#include <elf.h>
#include <cpio.h>
#include <string.h>
#include <mm/mm.h>
#include <stdio.h>
#include "syscall.h"

void sys_exit(int status)
{
    struct task_struct *current, *next;

    current = get_current();
    current->exit_status = status;
    current->status = TASK_STATUS_DEAD;

    disable_interrupt();
    run_queue_remove(current);
    next = run_queue_pop();   
    enable_interrupt();
    
    if (next != NULL) {
        switch_to(current, next);
    }
}

void sys_fork() 
{
    printf("sys fork \n");
}

void sys_exec(int status)
{
    printf("sys exec \n");
}

void sys_getpid()
{
    printf("sys getpid\n");
}

void *set_argv(void *stack, int argc, char* const argv[])
{
    char** argv_addr;
    int count = argc;

    if (count < 4)
           count = 4;

    disable_interrupt();
    argv_addr = (char**) kmalloc(count * sizeof(char*));
    enable_interrupt();

    for (int i = argc - 1; i >= 0; i--) {
        // padding
        int offset = (strlen(argv[i] + 1) % 16 == 0) ? strlen(argv[i]) + 1 : 16 * ((strlen(argv[i])/16 + 1));
        stack = stack - offset;
        argv_addr[i] = stack;

        memset((char*) stack, offset, 0);
        memcpy((char*) stack, argv[i], strlen(argv[i]) + 1);
    }

    stack = stack - sizeof(char*);
    *(char**)stack = NULL;

    for (int i = argc - 1; i >= 0; i--) {
        stack = stack - sizeof(char *);
        stack = argv_addr[i];
    }

    stack = stack - (16 - sizeof(int) % 16);
    stack = stack - sizeof(int);
    *(int*) stack = argc;

    return stack;
}

int do_exec(char *name, char* const argv[])
{
    void *addr, *start, *stack;
    char *arg;
    int argc;
    struct trapframe *current_tf;

    addr = load_file(name, strlen(name));
    if (addr == NULL)
           return -1;

    start = elf_header_parse(addr);
    printf("start %x\n", start);
    stack = get_current()->stack + TASK_STACK_SIZE;
    arg = argv[0];
    argc = 0;

    while (arg != NULL)
        arg = argv[argc++];

    set_argv(stack, argc, argv);

    current_tf = get_trapframe(get_current());

    current_tf->x0 = argc;
    current_tf->x1 = (size_t) stack + 0x10;
    current_tf->sp_el0 = (size_t)stack;
    current_tf->elr_el1 = (size_t)start;

    disable_interrupt();

    asm volatile("mov sp, %0\n" "blr %1\n"::"r"(current_tf), "r"((void*) kernel_exit)); //to-do

    enable_interrupt();

    return 0;
}

void sys_uart_read() {
    printf("uart read\n");
}

void sys_uart_write() {
    printf("uart write\n");
}

void sys_get_time() {
    printf("get time\n");
}
