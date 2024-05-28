#include <task_queue.h>
#include <sched.h>
#include <stdio.h>

void run_queue_push(struct task_struct *task)
{
    if (run_queue.tail == NULL) {
        run_queue.head = task;
        run_queue.tail = task;
    } else {
        run_queue.tail->next = task;
        run_queue.tail = task;
    }
    run_queue.tail->next =  NULL;
}

struct task_struct* run_queue_pop(void)
{  
    struct task_struct *ret;
    
    if (run_queue.head == NULL)
        return NULL;

    if(run_queue.head == run_queue.tail)
        run_queue.tail = NULL;
 
    ret = run_queue.head;
    run_queue.head = run_queue.head->next;
    ret->next = NULL;
       
    return ret;
}

void run_queue_remove(struct task_struct *del)
{
    struct task_struct *cur = run_queue.head, *prev = NULL;

    while (cur != NULL) {
        if (cur == del) {
            if (prev != NULL) {
                prev->next = cur->next;
                if (cur == run_queue.tail)
                    run_queue.tail = prev;
            }
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void run_queue_status(void)
{
    struct task_struct *cur = run_queue.head;

    if (cur == NULL)
        return;

    while (cur != NULL) {
        printf("task_id: %d ,", cur->task_id);
        cur = cur->next;
    }
    printf("\n");
}
