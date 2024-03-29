.section ".text"

#define CORE0_TIMER_IRQ_CTRL 0x40000040

.global core_timer_enable
core_timer_enable:
    mov x0, #1
    msr cntp_ctl_el0, x0
    mrs x0, cntfrq_el0
    lsl x0, x0, #2
    msr  cntp_tval_el0, x0
    mov x0, #2
    ldr  x1, =CORE0_TIMER_IRQ_CTRL
    str  w0, [x1]
    ret

.global core_timer_disable
core_timer_disable:
  mov  x0, #0
  msr  cntp_ctl_el0, x0 // disable
  ret 
