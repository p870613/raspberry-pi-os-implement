#include <type.h>
#include <mm/dynamic.h>
#include <mm/mm.h>
#include <mm/buddy.h>

void *kmalloc(size_t size)
{
    if(size > DYNAMIC_BIN_MAX * DYNAMIC_BIN_MIN_SLOT)
        return buddy_allocate(size);
    else
        return dynamic_malloc(size);
}

void kfree(void *address)
{
    size_t size, prev_size, order;
    void* next;
    
    size = ((struct dynamic_chunk*)(address - DYNAMIC_CHUNK_HEADER_OFFSET))->size;
    prev_size = ((struct dynamic_chunk*)(address - DYNAMIC_CHUNK_HEADER_OFFSET))->pre_size;
    next = ((struct dynamic_chunk*)(address - DYNAMIC_CHUNK_HEADER_OFFSET))->next;
    if (prev_size == 0 || prev_size * DYNAMIC_BIN_MIN_SLOT == 0) {
        if (next == NULL) {
            dynamic_free(address);
            return;
        }
    }

    order = ((struct buddy_block *)(address - BUDDY_HEADER_OFFSET))->order;
    next = ((struct buddy_block *)(address - BUDDY_HEADER_OFFSET))->next;

    if (order >= 0 && order <= BUDDY_MAX_ORDER) {
        if (next == NULL) {
            buddy_free(address);
            return;
        }
    }
    return;
}
