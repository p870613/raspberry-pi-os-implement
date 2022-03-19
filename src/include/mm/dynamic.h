#ifndef DYNAMIC_H
#define DYNAMIC_H

#include <type.h>

#define DYNAMIC_BIN_MAX 0X40
#define DYNAMIC_BIN_MIN_SLOT 0X10

#define DYNAMIC_CHUCK_HEADER_OFFSET 0X20

struct dynamic_struct {
    // top chuck
    struct dynamic_chuck *top_chuck;
    
    //free bin list
    struct dynamic_chuck *bin[DYNAMIC_BIN_MAX];

    //store chuck larger than DYNAMIC_BIN_MAX
    struct dynamic_chuck *unsort_bin;

} dynamic_system;

struct dynamic_chuck {
    size_t pre_size;
    size_t size;
    struct dynamic_chuck* next;
};

void dynamic_init();
void* dynamic_malloc(size_t);
void dynamic_free();
void* dynamic_find_free_chuck(size_t);
void* dynamic_find_free_chuck_from_small_bin(size_t);
void* dynamic_find_free_chuck_from_unsort_bin(size_t);
void* dynamic_find_free_chuck_from_top_chuck(size_t);

#endif
