#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H

#include <sched.h>
struct task_queue_struct {
    struct task_struct *head;
    struct task_struct *tail;
};

void run_queue_push(struct task_struct*);
void run_queue_status(void);
struct task_struct* run_queue_pop(void);

#endif
