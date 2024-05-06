#include "sched.h"

#include <stdio.h>
#include <task_queue.h>

// disable interrupt
// create task 
// enable interrupt
//
struct task_struct *task_create(void(*func)())
{
    int i = 0;

    for (i = 0; i < TASK_POOL_SIZE; i++)
        if (task_pool[i].task_id == 0)
            break;

    if (i == TASK_POOL_SIZE)
        return NULL;

    task_pool[i].task_id = i + 1;
    task_pool[i].status = TASK_STATUS_READY;
    task_pool[i].kstack = (void*)KERNEL_STACK_ADDR + TASK_STACK_SIZE * i;
    task_pool[i].ctx.sp = (size_t)task_pool[i].kstack + TASK_STACK_SIZE;
    task_pool[i].ctx.lr = (size_t) func;
    task_pool[i].stack = (void*)USER_STACK_ADDR + TASK_STACK_SIZE * i;
    task_pool[i].next = NULL;
    task_pool[i].start = NULL;
    task_pool[i].resched = 0;

    run_queue_push(&task_pool[i]);

    return &task_pool[i];
}

void idle_task()
{
    while(1) {
        for (int i = 0; i < TASK_POOL_SIZE; i++) {
            if(task_pool[i].status == TASK_STATUS_DEAD &&
               task_pool[i].task_id != 0) {
                // free start
                task_pool[i].start = NULL;
                task_pool[i].kstack = NULL;
                task_pool[i].stack = NULL;
                task_pool[i].task_id = 0;
            }
        }
        schedule();
    }
}

/*
void task_init()
{
    struct task_struct fake;


}
*/
