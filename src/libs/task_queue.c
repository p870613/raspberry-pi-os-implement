#include <task_queue.h>
#include <sched.h>

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
    ret->next = NULL;

    run_queue.head = run_queue.head->next;
       
    return ret;
}
