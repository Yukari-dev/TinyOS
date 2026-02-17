#ifndef FS_H
#define FS_H

#include "../libs/stdint.h"

typedef struct {
    uint8_t name[8];
    uint8_t ext[3];
    uint8_t attr;
    uint8_t reserved[10];
    uint16_t time;
    uint16_t date;
    uint16_t cluster;
    uint32_t size;
} __attribute__((packed)) DirectoryEntry;

void fs_init();
uint8_t* fs_load_file(char* name, uint32_t* out_size);

#endif
