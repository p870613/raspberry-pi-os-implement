#ifndef _SCHED_H
#define _SCHED_H

#include <task_queue.h>
#include <type.h>

#define TASK_POOL_SIZE      0X40
#define TASK_STACK_SIZE     0X1000
#define KERNEL_STACK_ADDR   0x11000000
#define USER_STACK_ADDR     (11000000 + TASK_STACK_SIZE * TASK_POOL_SIZE)

enum task_status {
    TASK_STATUS_DEAD,
    TASK_STATUS_READY,
};


struct trapframe { // user space
  size_t x0, x1, x2, x3, x4, x5, x6, x7, x8, x9;
  size_t x10, x11, x12, x13, x14, x15, x16, x17, x18, x19;
  size_t x20, x21, x22, x23, x24, x25, x26, x27, x28, x29;
  size_t x30, sp_el0, elr_el1, spsr_el1;
};

struct context {
  size_t x19;
  size_t x20;
  size_t x21;
  size_t x22;
  size_t x23;
  size_t x24;
  size_t x25;
  size_t x26;
  size_t x27;
  size_t x28;
  size_t fp; //frame pointer, stack base address
  size_t lr; //return address
  size_t sp; //stack pointer
};


struct task_struct {
    struct context ctx; // before task
    size_t task_id;
    void *kstack;
    int status;
    int exit_status;
    int resched;
    void* start;
    size_t size;
    void *stack;
    struct task_struct *next;    
};

struct task_struct task_pool[TASK_POOL_SIZE];
struct task_queue_struct run_queue;

void schedule(void);
extern void disable_interrupt(void);
extern void enable_interrupt(void);
extern void switch_to(struct task_struct*, struct task_struct*);
extern struct task_struct* get_current(void);
#endif
