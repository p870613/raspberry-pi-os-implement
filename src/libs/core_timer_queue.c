#include "core_timer_queue.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void core_timer_queue_init() {
    core_timer_callback_queue = NULL;
}
void core_timer_interrupt_handler() {
    core_timer_queue_update();
    struct core_timer_callback* pop_node = NULL;

    while(1) {
        pop_node = core_timer_queue_pop();
        if(pop_node == NULL){
            break;
        } else {
            ((void(*)(char*))(pop_node->callback_function))(pop_node->buf);
            free(pop_node);
        }
    }
    return ;
}

void core_timer_queue_push(void* callback, int timeout, char* mes, int size) {
    struct core_timer_callback* node = (struct core_timer_callback*) malloc (sizeof(struct  core_timer_callback));
    printk("core timer queue push finish\n");
    node->callback_function = callback;
    node->timeout = timeout;
    node->argc = 0;
    node->argv = NULL;
    node->size = size;
    node->next = NULL;
    strncpy(node->buf, mes, size);
    if(core_timer_callback_queue == NULL) {
        core_timer_callback_queue = node;
    } else {
        node->next = core_timer_callback_queue;
        core_timer_callback_queue = node;
    }
    core_timer_queue_sort();
    core_timer_queue_status(); 
}

void* core_timer_queue_pop() {
    struct core_timer_callback* ret = core_timer_callback_queue;
    
    if(ret != NULL) {
        if(ret -> timeout == 0) {
            core_timer_callback_queue = core_timer_callback_queue -> next;
            return ret;
        }
    }
    return NULL;
}

void core_timer_queue_update() {
    struct core_timer_callback* cur = core_timer_callback_queue;
    while(cur != NULL) {
        cur -> timeout = (cur->timeout) - 1;
        cur = cur->next;
    }
}

void core_timer_queue_sort() {
    struct core_timer_callback* cur, *pre, *next;
    cur = core_timer_callback_queue;
    pre = NULL;
    
    if(cur != NULL)
        next = cur->next;
    else 
        next = NULL;
    
    while(cur != NULL) {
        if(next != NULL) {
            if(cur->timeout > next->timeout) {
    
                cur->next = next->next;
                next->next = cur;
                
                if(pre != NULL) 
                    pre->next = next;
                else 
                    core_timer_callback_queue = next;
                cur = core_timer_callback_queue;
            }
        }
        pre = cur;
        cur = cur->next;
           
        if(cur != NULL) 
            next = cur->next;
        else 
            next = NULL;
    }
    return ;
}

void core_timer_queue_status() {
    struct core_timer_callback* cur = core_timer_callback_queue;
    
    while(cur != NULL) {
        printk("timeout: %d ", cur->timeout);
        cur = cur -> next;
    }
    printk("\n");
}

void* core_timer_print_message(char* str) {
    printf("core time: %f, message: %s", get_core_time(), str);
    return NULL;
}
