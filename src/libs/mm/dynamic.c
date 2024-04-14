#include <mm/dynamic.h>
#include <mm/buddy.h>

void dynamic_init () {
    if (dynamic_system.top_chunk == NULL) {
        dynamic_system.top_chunk = buddy_allocate(PAGE_SIZE);
        dynamic_system.top_chunk->size = PAGE_SIZE - BUDDY_HEADER_OFFSET - DYNAMIC_CHUNK_HEADER_OFFSET;
        dynamic_system.top_chunk->next = NULL;
        dynamic_system.top_chunk->pre_size = 1;
    }
}

void* dynamic_malloc (size_t size) {
    struct dynamic_chunk* ret_chunk;
    size_t bin_index;

    for(size_t i = 0; i < DYNAMIC_BIN_MAX; i++) {
        if (size <= (i + 1) * DYNAMIC_BIN_MIN_SLOT) {
            bin_index = i;
            break;
        }
    }
    ret_chunk = dynamic_find_free_chunk(bin_index);
    ret_chunk->size ++;
    
    return (void*)(ret_chunk) + DYNAMIC_CHUNK_HEADER_OFFSET;
}

void dynamic_merge_chunk(struct dynamic_chunk* cur, struct dynamic_chunk* pre, struct dynamic_chunk* next) {
    if((pre->size & 0x01) == 1)
        return;
    
    int merge_size = cur->size + pre->size + DYNAMIC_CHUNK_HEADER_OFFSET;
    if(merge_size <= DYNAMIC_BIN_MAX * DYNAMIC_BIN_MIN_SLOT) {
        int pre_bin_index = pre->size / DYNAMIC_BIN_MIN_SLOT - 1;
        int merge_bin_index = merge_size / DYNAMIC_BIN_MIN_SLOT -1;
        struct dynamic_chunk* now = dynamic_system.bin[pre_bin_index];
        struct dynamic_chunk* now_pre = NULL;
        
        while(now != NULL) {
            if(now == pre) {
                if(now_pre == NULL) {
                    dynamic_system.bin[pre_bin_index] = now->next;
                } else {
                    now_pre->next = now->next;
                }
                pre->next = dynamic_system.bin[merge_bin_index];
                dynamic_system.bin[merge_bin_index] = pre;
                pre->size = merge_size;
                next->pre_size = merge_size;
                return ;
            }
            now_pre = now;
            now = now -> next;
        }
    } else {
        int pre_bin_index = pre->size / DYNAMIC_BIN_MIN_SLOT - 1;
        struct dynamic_chunk* now;
        struct dynamic_chunk* now_pre = NULL;
        
        if(pre_bin_index < DYNAMIC_BIN_MAX) {
            now = dynamic_system.bin[pre_bin_index];
            while(now != NULL) {
                if(now == pre) {
                    if(now_pre == NULL) {
                        dynamic_system.bin[pre_bin_index] = now->next;
                    } else {
                        now_pre->next = now->next;
                    }
                    pre->next = dynamic_system.unsort_bin;
                    dynamic_system.unsort_bin = pre;
                    pre->size = merge_size;
                    next->pre_size = merge_size;
                    return;
                }
                now_pre = now;
                now = now -> next;
            }
        } else {
            now = dynamic_system.unsort_bin;
            while(now != NULL) {
                if(now == pre) {
                    if(now_pre == NULL) {
                        dynamic_system.unsort_bin = now->next;
                    } else {
                        now_pre->next = now->next;
                    }
                    pre->next = dynamic_system.unsort_bin;
                    dynamic_system.unsort_bin = pre;
                    pre->size = merge_size;
                    next->pre_size = merge_size;
                }
                now_pre = now;
                now = now->next;
            }
        }
    }
}

void dynamic_free (void* address) {
    address = address - DYNAMIC_CHUNK_HEADER_OFFSET;
    if(address < buddy_system_header.start || address > buddy_system_header.end)
        return;
    
    struct dynamic_chunk *chunk = address;
    chunk->size --;
    struct dynamic_chunk *pre_chunk = address - chunk->pre_size - DYNAMIC_CHUNK_HEADER_OFFSET;
    struct dynamic_chunk *next_chunk = address + chunk->size + DYNAMIC_CHUNK_HEADER_OFFSET;
    size_t size = chunk->size;
    int bin_index = size / DYNAMIC_BIN_MIN_SLOT - 1;

    if(chunk->size >= DYNAMIC_BIN_MIN_SLOT && chunk->size <= DYNAMIC_BIN_MAX * DYNAMIC_BIN_MIN_SLOT) {
        if( (chunk->pre_size != 1) &&((pre_chunk->size & 0x01) == 0) 
                && (pre_chunk >= ((unsigned int)address & 0xfffff000))) {
            dynamic_merge_chunk(chunk, pre_chunk, next_chunk);
        } else {
            chunk->next = dynamic_system.bin[bin_index];
            dynamic_system.bin[bin_index] = chunk;
        }
    } else {
        chunk->next = dynamic_system.unsort_bin;
        dynamic_system.unsort_bin = chunk;
    }
}

void* dynamic_find_free_chunk (size_t bin_index) {
    void* ret = dynamic_find_free_chunk_from_small_bin(bin_index);
    
    if(ret == NULL)
        ret = dynamic_find_free_chunk_from_unsort_bin(bin_index);
   
    if(ret == NULL)
        ret = dynamic_find_free_chunk_from_top_chunk(bin_index);
    
    return ret;
}

void* dynamic_find_free_chunk_from_small_bin(size_t bin_index) {
    if(bin_index > DYNAMIC_BIN_MAX) {
        return NULL;
    }
    
    struct dynamic_chunk* ret;
    if(dynamic_system.bin[bin_index] != NULL) {
        ret = dynamic_system.bin[bin_index];
        dynamic_system.bin[bin_index] = dynamic_system.bin[bin_index]->next;
        return ret;
    } else {
        return NULL;
    }
}

int remove_from_unsort_bin(void* address, size_t size) {
    struct dynamic_chunk* cur_chunk = dynamic_system.unsort_bin;
    struct dynamic_chunk* pre_chunk = NULL;
    
    while (cur_chunk != NULL) {
        if(cur_chunk == address) {
            if(pre_chunk == NULL) 
                dynamic_system.unsort_bin = cur_chunk->next;
            else 
                pre_chunk->next = cur_chunk->next;
            uart_put("Remove chuck from unsort bin\n");
            return 0;
        } else {
            pre_chunk = cur_chunk;
            cur_chunk = cur_chunk->next;
        }
    }
    return -1;
}

struct dynamic_chunk* unsort_bin_split_chunk(struct dynamic_chunk* chunk, size_t size) {
    if(chunk->size == size)
        return ;
   
    struct dynamic_chunk *split_chunk = (void*) chunk + size;
    split_chunk->size = size;
    
    chunk->pre_size = size;
    chunk->size = chunk->size - size;
    
    if(chunk->size <= DYNAMIC_BIN_MAX * DYNAMIC_BIN_MIN_SLOT) {
        // put samll bin
        int bin_index = chunk->size / DYNAMIC_BIN_MIN_SLOT - 1;
        chunk->next = dynamic_system.bin[bin_index];
        dynamic_system.bin[bin_index] = chunk;
    } else {
        // put unsort bin
        chunk->next = dynamic_system.unsort_bin;
        dynamic_system.unsort_bin = chunk;
    }
    return split_chunk;
}

void* dynamic_find_free_chunk_from_unsort_bin(size_t bin_index) {
    struct dynamic_chunk *cur_chunk, *pre_chunk;
    size_t size = (bin_index + 1) * DYNAMIC_BIN_MIN_SLOT + DYNAMIC_CHUNK_HEADER_OFFSET;
    
    cur_chunk = dynamic_system.unsort_bin;
    pre_chunk = NULL;
    while (cur_chunk != NULL) {
        if(size <= cur_chunk->size) {
            if (remove_from_unsort_bin(cur_chunk, size) == -1) {
                uart_put("Remove from unsort bin fail\n");
                return NULL;
            } else {
                struct dynamic_chunk *chunk = unsort_bin_split_chunk(cur_chunk, size);                
                return chunk; 
            }
        }
        pre_chunk = cur_chunk;
        cur_chunk = cur_chunk->next;
    }

    uart_put("No free chunk in unsort bin\n");
    return NULL;
}

void top_chunk_free() {
    size_t size = dynamic_system.top_chunk->size;

    if(size >= DYNAMIC_BIN_MIN_SLOT) {
        if(size <= DYNAMIC_BIN_MAX * DYNAMIC_BIN_MIN_SLOT) {
            int bin_index = size / DYNAMIC_BIN_MIN_SLOT - 1;
            dynamic_system.top_chunk->next = dynamic_system.bin[bin_index];
            dynamic_system.bin[bin_index] = dynamic_system.top_chunk;
        } else {
            dynamic_system.top_chunk->next = dynamic_system.unsort_bin;
            dynamic_system.unsort_bin = dynamic_system.top_chunk;
        }
    }
}

int dynamic_request_new_page() {
    void* tmp = buddy_allocate(PAGE_SIZE);
    if(tmp == NULL) {
        uart_put("Request from buddy fail\n");
        return -1;
    }
    
    top_chunk_free();
    
    dynamic_system.top_chunk = tmp;
    dynamic_system.top_chunk->size = PAGE_SIZE - BUDDY_HEADER_OFFSET - DYNAMIC_CHUNK_HEADER_OFFSET;
    dynamic_system.top_chunk->next = NULL;
    dynamic_system.top_chunk->pre_size = 1;

    return 0;
}

void* dynamic_find_free_chunk_from_top_chunk(size_t bin_index) {
    if(dynamic_system.top_chunk->size < (bin_index + 1) * DYNAMIC_BIN_MIN_SLOT)
        if(dynamic_request_new_page() == -1) 
            return NULL;
    
    struct dynamic_chunk *chunk = dynamic_system.top_chunk; 
    size_t chunk_size = (bin_index + 1) * DYNAMIC_BIN_MIN_SLOT;
    chunk->size = chunk_size;
    size_t top_chunk_size = dynamic_system.top_chunk->size;

    dynamic_system.top_chunk = (void *)dynamic_system.top_chunk + chunk_size + DYNAMIC_CHUNK_HEADER_OFFSET;
    dynamic_system.top_chunk->size = top_chunk_size - chunk_size - DYNAMIC_CHUNK_HEADER_OFFSET;
    dynamic_system.top_chunk->next = NULL;
    dynamic_system.top_chunk->pre_size = chunk_size;

    return chunk;
}

void dynamic_status() {
    uart_put("dynamic status: \n");
    uart_put("free list: \n");

    struct dynamic_chunk* chunk;
    for(int i = 0; i < DYNAMIC_BIN_MAX; i++) {
        chunk = dynamic_system.bin[i];

        if(chunk == NULL) {
            uart_hex(i);
            uart_put(": NO list\n");
            continue;
        }

        uart_hex(i);
        uart_put(": ");
        while(chunk != NULL) {
            uart_hex((size_t)chunk);
            uart_put(" --> ");
            chunk = chunk->next;
        }

        uart_put("null\n");
    }

    uart_put("unsorted bin info:\n");
    chunk = dynamic_system.unsort_bin;
    while(chunk != NULL) {
        uart_hex((size_t)chunk);
        uart_put("[");
        uart_hex(chunk->size);
        uart_put("] --> ");
        chunk = chunk->next;
    }
    uart_put("null\n");
    
    uart_put("top chunk info: \n");
    uart_put("address: ");
    uart_hex((size_t)dynamic_system.top_chunk);
    uart_put("\n");
    uart_put("size: ");
    uart_hex(dynamic_system.top_chunk->size);
    uart_put("\n");
}

void dynamic_test()
{
    char *ptr[4];
    char *p;
   
    p = dynamic_malloc(0x10);
    ptr[0] = dynamic_malloc(0x10);
    ptr[1] = dynamic_malloc(0x20);
    ptr[2] = dynamic_malloc(0x380);
    ptr[3] = dynamic_malloc(0x400);
        

    dynamic_free(p);
    /*dynamic_free(ptr[0]);*/
    /*dynamic_free(ptr[1]);*/
    /*dynamic_free(ptr[2]);*/
    /*dynamic_free(ptr[3]);*/
    
    dynamic_status();
    p = dynamic_malloc(0x10);
    printf("%x\n", p); 
    dynamic_status();
    return;
}
