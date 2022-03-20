#ifndef _BUDDY_H
#define _BUDDY_H

#include <mm/mm.h>
#include <type.h>
#include <string.h>

#define BUDDY_MAX_ORDER 9
#define BUDDY_BLOCK_NUM (1 << BUDDY_MAX_ORDER)
#define BUDDY_START 0X10000000
#define BUDDY_HEADER_OFFSET 0X10
#define BUDDY_START 0X10000000
//#define BUDDY_START_HEADER 0X10000000
//#define BUDDY_START_BLOCK  (0X10000000 + BUDDY_BLOCK_NUM * BUDDY_HEADER_OFFSET)

struct buddy_header_data {
    void* start;
    void* end;
    size_t size;
    
    char inuse[BUDDY_BLOCK_NUM + 1];
    struct buddy_block* free_list[BUDDY_MAX_ORDER+1];
};

struct buddy_block {
    size_t order;
    struct buddy_block* next;
};

struct buddy_header_data buddy_system_header;

void buddy_init();
void* buddy_allocate(size_t);
void buddy_free(void* address);
void print_free_list();
#endif
