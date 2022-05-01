#include <stdio.h>
#include <type.h>
#include <exception.h>
void exception_level_info() {
  int32_t spsr_el1, esr_el1, cur_el;
  size_t elr_el1;
  asm volatile("mrs  %0, spsr_el1": "=r"(spsr_el1));
  printf("spsr_el1: %x\n", spsr_el1);
  asm volatile("mrs  %0, elr_el1\n": "=r"(elr_el1));
  printf("elr_el1: %x\n", elr_el1);
  asm volatile("mrs  %0, esr_el1\n": "=r"(esr_el1));
  printf("esr_el1: %x\n", esr_el1);
  /*asm volatile("mrs  %0, CurrentEL\n": "=r"(cur_el));*/
  /*printf("CurrentEL: %x\n", cur_el);*/
}
