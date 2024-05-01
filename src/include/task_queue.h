#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H

struct task_queue_struct {
    struct task_struct *head;
    struct task_struct *tail;
};
#endif
