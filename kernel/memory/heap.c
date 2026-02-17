#include "heap.h"

static uint8_t* heap_ptr;

void heap_init() {
    heap_ptr = (uint8_t*)0x400000; 
}

void* malloc(uint32_t size) {
    void* res = heap_ptr;
    heap_ptr += size;
    return res;
}
