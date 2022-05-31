#ifndef _CORE_TIMER_QUEUE_H
#define _CORE_TIMER_QUEUE_H

#include <type.h>

#define CORE_TIMER_CALLBACK_SIZE 0X100

typedef struct core_timer_callback {
    void* callback_function;
    void* argv;
    size_t argc;
    size_t timeout;
    char buf[CORE_TIMER_CALLBACK_SIZE];
    size_t size;
    struct core_timer_callback* next;
};

struct core_timer_callback* core_timer_callback_queue;

void core_timer_interrupt_handler();
void core_timer_queue_push(void*, int, char*, int);
void* core_timer_queue_pop();
void core_timer_queue_update();
void core_timer_queue_sort();
void core_timer_queue_status();
void* core_timer_print_message(char*);

#endif
