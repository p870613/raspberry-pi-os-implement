#include <stdlib.h>
#include <mm/dynamic.h>

void* malloc(size_t size) {
    void* ret = dynamic_malloc(size);
    if(ret == NULL) {
        return NULL;
    } else {
        ret = ret + DYNAMIC_CHUNK_HEADER_OFFSET;
        return ret;
    }
}

void free(void* ar) {
    if(ar != NULL) {
        dynamic_free(ar - DYNAMIC_CHUNK_HEADER_OFFSET);
        return;
    } else {
        return;
    }
}
