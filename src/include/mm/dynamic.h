#ifndef DYNAMIC_H
#define DYNAMIC_H

#include <type.h>

#define DYNAMIC_BIN_MAX 0X40
#define DYNAMIC_BIN_MIN_SLOT 0X10

#define DYNAMIC_CHUNK_HEADER_OFFSET 0X20

struct dynamic_struct {
    // top chuck
    struct dynamic_chunk *top_chunk;
    
    //free bin list
    struct dynamic_chunk *bin[DYNAMIC_BIN_MAX];

    //store chuck larger than DYNAMIC_BIN_MAX
    struct dynamic_chunk *unsort_bin;

} dynamic_system;

struct dynamic_chunk {
    size_t pre_size;
    size_t size;
    char use;
    struct dynamic_chunk* next;
};

void dynamic_init();
void* dynamic_malloc(size_t);
void dynamic_free(void*);
void* dynamic_find_free_chunk(size_t);
void* dynamic_find_free_chunk_from_small_bin(size_t);
void* dynamic_find_free_chunk_from_unsort_bin(size_t);
void* dynamic_find_free_chunk_from_top_chunk(size_t);
void dynamic_status();

#endif
