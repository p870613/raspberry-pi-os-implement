#include <mm/buddy.h>
#include <mm/mm.h>
#include <stdio.h>

int get_order(size_t);
int get_index(void* );
int is_free(size_t, int);
int remove_free_list_block(int ,void* );
void buddy_reset_block(void*, int);
void merge(void* address);
void* buddy_find_free_block(int);
void update_inuse(struct buddy_block* remain, int order);

void buddy_init() {
    buddy_system_header.start = (void*) BUDDY_START;
    buddy_system_header.size = (PAGE_SIZE + BUDDY_HEADER_OFFSET) * BUDDY_BLOCK_NUM;
    buddy_system_header.end = buddy_system_header.start + buddy_system_header.size;
    
    ((struct buddy_block*) buddy_system_header.start) -> next = NULL;
    ((struct buddy_block*) buddy_system_header.start) -> order = BUDDY_MAX_ORDER;
    buddy_system_header.inuse[0] = BUDDY_MAX_ORDER;
    buddy_system_header.free_list[BUDDY_MAX_ORDER] = buddy_system_header.start;
    memset(buddy_system_header.inuse + 1, 'F', (size_t)BUDDY_BLOCK_NUM - 1);
}

int get_order(size_t size) {
    int re = 0;
    while(size > PAGE_SIZE * (1 << re)) 
        re ++;
    return re;
}

void* buddy_allocate(size_t size) {
    int order = get_order(size);
    if(order > BUDDY_MAX_ORDER) {
        uart_put("order too big\n");
        return NULL;
    }
        
    void* block = buddy_find_free_block(order);
    
    if(block != NULL) {
        ((struct buddy_block* )block) -> order = order;
        ((struct buddy_block* )block) -> next = NULL;
        update_inuse(block, order);
    }
    return block + BUDDY_HEADER_OFFSET;
}

void buddy_free(void* address) {
    address = address - BUDDY_HEADER_OFFSET;
    if(address < buddy_system_header.start || address > buddy_system_header.end)
        return ;
    merge(address);
}

int get_index(void* address) {
    return ((address - buddy_system_header.start) / (PAGE_SIZE + BUDDY_HEADER_OFFSET));
}


int is_free(size_t index, int order) {
    size_t size = (1 << order);
    for(int i = 0; i < size; i++) {
        if(buddy_system_header.inuse[index + i] == 'X' )
            return 0;
    }
    return 1;
}

int remove_free_list_block(int order, void* address) {
   struct buddy_block* cur = buddy_system_header.free_list[order];
   struct buddy_block* pre = NULL ;

    while(cur != NULL) {
        if(cur == (struct buddy_block*) address) {
            if(pre == NULL) {
                buddy_system_header.free_list[order] = cur -> next;
            } else {
                pre -> next = cur -> next;
            }
            return 1;
        }

        pre = cur;
        cur = cur -> next;
    } 
    return 0;
}

void buddy_reset_block(void* address, int order) {
    size_t size = (1 << order);
    int index = get_index(address);
    buddy_system_header.inuse[index] = order;
    memset(buddy_system_header.inuse + index + 1, 'F', size - 1);
}

void merge(void* address) {
    struct buddy_block* merge_block;
    struct buddy_block* check_block;
    int merge_order, merge_index;
    int check_block_index;

    merge_block = address;
    merge_order = merge_block -> order;
    merge_index = get_index(address);

    while(merge_order < BUDDY_MAX_ORDER) {
        check_block_index = merge_index ^ (1 << merge_order);
        check_block = buddy_system_header.start + (BUDDY_HEADER_OFFSET + PAGE_SIZE) * check_block_index;
        
        if(is_free(check_block_index, merge_order) == 1) {
            uart_int(merge_order);
            uart_put("merge block free\n");

            if(remove_free_list_block(merge_order, check_block) == 0) {
                uart_put("remove free list error\n");
                break;
            }

            if(merge_index > check_block_index) {
                merge_index = check_block_index;
                merge_block = check_block;
            }
            merge_order ++;
        } else {
            break;
        }
    }
    merge_block -> next = buddy_system_header.free_list[merge_order];
    buddy_system_header.free_list[merge_order] = merge_block;
    merge_block -> order = merge_order;

    buddy_reset_block(merge_block, merge_order);
}


void* buddy_find_free_block(int order) {
    if(order > BUDDY_MAX_ORDER) 
        return NULL;
    
    void* find_block;
    
    if(buddy_system_header.free_list[order] != NULL) {
        find_block = buddy_system_header.free_list[order];
        buddy_system_header.free_list[order] = buddy_system_header.free_list[order]->next;
        return find_block;
    } else {
        find_block = buddy_find_free_block(order + 1);
    }

    if(find_block != NULL) {
        struct buddy_block* remain;
        remain = find_block + (PAGE_SIZE + BUDDY_HEADER_OFFSET) * (1 << order);
        remain -> next = buddy_system_header.free_list[order];
        remain -> order = order;
        buddy_system_header.free_list[order] = remain;
        
        update_inuse(remain, order);
    }
    return find_block;
}

void update_inuse(struct buddy_block* remain, int order) {
    int index = ((size_t) remain - (size_t)buddy_system_header.start) / (PAGE_SIZE * BUDDY_HEADER_OFFSET);
    buddy_system_header.inuse[index] = order;
    memset(buddy_system_header.inuse + index + 1, 'F', 1 << order);
}

void traverse_list(struct buddy_block* head) {
    struct buddy_block* cur = head;
    if(head == NULL) {
        uart_put(" no free list\n");
        return;
    }
    while(cur != NULL) {
        uart_put("[");
        uart_int(cur -> order);
        uart_put(",");
        uart_hex((unsigned long long)cur);
        uart_put("], ");
        cur = cur -> next;
    }
    uart_put("\n");
}
void print_free_list() {
    for(int i = 0; i <= BUDDY_MAX_ORDER; i++) {
        uart_int(i);
        traverse_list(buddy_system_header.free_list[i]);
    }
}

void buddy_test()
{
    void* a= buddy_allocate(4098);
    print_free_list();
    void *b = buddy_allocate(20);
    print_free_list();

    buddy_free(a);
    print_free_list();
    buddy_free(b);
    print_free_list();

}
