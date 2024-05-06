#include <sched.h>
#include "task_queue.h"

void schedule(void)
{
    struct task_struct *cur, *next;

    cur = get_current();
    if (cur->resched == 1) {
        disable_interrupt();

        next = run_queue_pop();
        run_queue_push(cur);
        cur->resched = 0;
        enable_interrupt();

        if (next != NULL)
            switch_to(cur, next);
    }
}
