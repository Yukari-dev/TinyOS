#ifndef HEAD_H
#define HEAD_H
#define HEAP_START 0x800000;

typedef struct Header{
    unsigned int size;
    unsigned char is_free;
    struct Header* next;
} Header;

extern Header* first_block;

void init_heap();

void* malloc(unsigned int size);

void free(void* ptr);

#endif
