#include "heap.h"

Header* first_block = (Header*)HEAP_START;

void init_heap(){
    first_block->size = 1024 * 1024;
    first_block->is_free = 1;
    first_block->next = 0;
}

void* malloc(unsigned int size){
    Header* curr = first_block;

    while(curr != 0){
        if(curr->is_free && curr->size >= size){
            curr->is_free = 0;

            return (void*)(curr + 1);
        }
        curr = curr->next;
    }

    return 0;
}

void free(void* ptr){
    if(!ptr) return;

    Header* header = (Header*)ptr - 1;

    header->is_free = 1;

    if(header->next && header->next->is_free){
        header->size += sizeof(Header) + header->next->size;
        header->next = header->next->next;
    }

}

