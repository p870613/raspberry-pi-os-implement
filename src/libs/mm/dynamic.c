#include <mm/dynamic.h>
#include <mm/buddy.h>

void dynamic_init () {
    if (dynamic_system.top_chuck == NULL) {
        dynamic_system.top_chuck = buddy_allocate(PAGE_SIZE);
        dynamic_system.top_chuck->size = PAGE_SIZE - BUDDY_HEADER_OFFSET - DYNAMIC_CHUCK_HEADER_OFFSET;
        dynamic_system.top_chuck->next = NULL;
        dynamic_system.top_chuck->pre_size = 0;
    }
}

void* dynamic_malloc (size_t size) {
    void* ret_chuck;
    size_t bin_index;

    for(size_t i = 0; i < DYNAMIC_BIN_MAX; i++) {
        if (size > (i + 1) * DYNAMIC_BIM_MIN_SLOT) {
            bin_index = i;
            break;
        }
    }
    
    if((bin_index + 1) * DYNAMIC_BIN_MIN_SLOT < DYNAMIC_CHUNK_HEADER_OFFSET) {
        uart_put("size too small, it must be larger than ");
        uart_hex(DYNAMIC_CHUCK_HEADER_OFFSET);
        uart_put(".\n");
        return NULL;
    } 
    ret_chuck = dynamic_find_free_chuck(bin_index);
    return ret_chuck;


}

// 1. unsort bin
// 2. small bin
// 3. top chuck
void* dynamic_find_free_chuck (size_t bin_index) {
    void* ret = dynamic_find_free_chuck_from_unsort_bin((bin_index + 1 ) * DYNAMIC_BIN_MIN_SLOT);
   
    if(ret == NULL)
        ret = dynamic_find_free_chuck_from_small_bin(bin_index);

    if(ret == NULL)
        ret = dynamic_find_free_chuck_from_top_chuck(bin_index);

    return ret;
}

void dynamic_free () {

}

void* dynamic_find_free_chuck() {

}





