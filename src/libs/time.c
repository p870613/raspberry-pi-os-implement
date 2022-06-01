#include <time.h>
#include <type.h>

float get_core_time() {
    float ret;
    size_t count, freq;
    asm volatile("mrs %[result], cntpct_el0": [result]"=r"(count));
    asm volatile("mrs %[result], cntfrq_el0": [result]"=r"(freq));
    ret = (1000*count) / freq;
    return ret;
}
