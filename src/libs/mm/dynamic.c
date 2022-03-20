#include <mm/dynamic.h>
#include <mm/buddy.h>

void dynamic_init () {
    if (dynamic_system.top_chunk == NULL) {
        dynamic_system.top_chunk = buddy_allocate(PAGE_SIZE);
        dynamic_system.top_chunk->size = PAGE_SIZE - BUDDY_HEADER_OFFSET - DYNAMIC_CHUNK_HEADER_OFFSET;
        dynamic_system.top_chunk->next = NULL;
        dynamic_system.top_chunk->pre_size = 0;
    }
}

void* dynamic_malloc (size_t size) {
    struct dynamic_chunk* ret_chuck;
    size_t bin_index;

    for(size_t i = 0; i < DYNAMIC_BIN_MAX; i++) {
        if (size < (i + 1) * DYNAMIC_BIN_MIN_SLOT) {
            bin_index = i;
            break;
        }
    }
    uart_hex(bin_index);    
    if((bin_index + 1) * DYNAMIC_BIN_MIN_SLOT < DYNAMIC_CHUNK_HEADER_OFFSET) {
        uart_put("size too small, it must be larger than ");
        uart_hex(DYNAMIC_CHUNK_HEADER_OFFSET);
        uart_put(".\n");
        return NULL;
    } 
    ret_chuck = dynamic_find_free_chunk(bin_index);
    ret_chuck->size ++; //inuse bit
    return (void*)ret_chuck;
}

void dynamic_free (void* address) {
    if(address < buddy_system_header.start || address > buddy_system_header.end)
        return;
    
    struct dynamic_chunk *chunk = address;
    struct dynamic_chunk *pre_chunk = address - chunk->pre_size - DYNAMIC_CHUNK_HEADER_OFFSET;
    struct dynamic_chunk *next_chunk = address + chunk->size + DYNAMIC_CHUNK_HEADER_OFFSET;
    size_t size = chunk->size - 1;
    int bin_index = size / DYNAMIC_BIN_MIN_SLOT - 1;

    if(chunk->size >= DYNAMIC_BIN_MIN_SLOT && chunk->size <= DYNAMIC_BIN_MAX * DYNAMIC_BIN_MIN_SLOT) {
        // chunk => small bin
        // pre chunk 
        // next chunk
        
        // no merge
        chunk->next = dynamic_system.bin[bin_index];
        dynamic_system.bin[bin_index] = chunk;
    } else {
        //chunk => unsort bin
        chunk->next = dynamic_system.unsort_bin;
        dynamic_system.unsort_bin = chunk;
        
    }




}

// 1. unsort bin
// 2. small bin
// 3. top chuck
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
            uart_put("Remove chuck from unsort bin");
            return 0;
        } else {
            pre_chunk = cur_chunk;
            cur_chunk = cur_chunk->next;
        }
    }
    return -1;
}

void unsort_bin_split_chunk(struct dynamic_chunk* chunk, size_t size) {
    if(chunk->size == size)
        return ;
    
    struct dynamic_chunk *split_chunk = (void*) chunk + size + DYNAMIC_CHUNK_HEADER_OFFSET;
    split_chunk->size = chunk->size - size;
    split_chunk->pre_size = size;
    
    if(split_chunk->size <= DYNAMIC_BIN_MAX * DYNAMIC_BIN_MIN_SLOT) {
        // put samll bin
        int bin_index = split_chunk->size / DYNAMIC_BIN_MIN_SLOT - 1;
        split_chunk->next = dynamic_system.bin[bin_index];
        dynamic_system.bin[bin_index] = split_chunk;
    } else {
        // put unsort bin
        split_chunk->next = chunk;
        dynamic_system.unsort_bin = split_chunk;
    }
}

void* dynamic_find_free_chunk_from_unsort_bin(size_t bin_index) {
    struct dynamic_chunk *cur_chunk, *pre_chunk;
    size_t size = (bin_index + 1) * DYNAMIC_BIN_MIN_SLOT;
    cur_chunk = dynamic_system.unsort_bin;
    pre_chunk = NULL;

    while (cur_chunk != NULL) {
        if(size <= cur_chunk->size) {
            if (remove_from_unsort_bin(cur_chunk, size) == -1) {
                uart_put("Remove from unsort bin fail\n");
                return NULL;
            } else {
                unsort_bin_split_chunk(cur_chunk, size);                
                return cur_chunk; 
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
    
    // top_chunk to unsort bin or small bin
    top_chunk_free();
    
    // reset top chunk
    dynamic_system.top_chunk = tmp;
    dynamic_system.top_chunk->size = PAGE_SIZE - DYNAMIC_CHUNK_HEADER_OFFSET;
    dynamic_system.top_chunk->next = NULL;
    dynamic_system.top_chunk->pre_size = 0;

    return 0;
}
void* dynamic_find_free_chunk_from_top_chunk(size_t bin_index) {
    if(dynamic_system.top_chunk->size < (bin_index + 1) * DYNAMIC_BIN_MIN_SLOT)
        if(dynamic_request_new_page() == -1) 
            return NULL;
    
    void* chunk = dynamic_system.top_chunk; 
    size_t chunk_size = (bin_index + 1) * DYNAMIC_BIN_MIN_SLOT; 
    size_t top_chunk_size = dynamic_system.top_chunk->size;

    dynamic_system.top_chunk->size = chunk_size;
    dynamic_system.top_chunk = (void *)dynamic_system.top_chunk + chunk_size + DYNAMIC_CHUNK_HEADER_OFFSET;
    dynamic_system.top_chunk = chunk + chunk_size + DYNAMIC_CHUNK_HEADER_OFFSET;
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
