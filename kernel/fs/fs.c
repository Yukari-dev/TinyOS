#include "fs.h"
#include "../drivers/disk.h"
#include "../memory/heap.h"
#include "../libs/string.h"

// THE DEFINITIONS
#define SLAVE_DRIVE 1
#define PARTITION_OFFSET 1 
#define RESERVED_SECTORS 1 
#define FAT_SIZE 9
#define NUM_FATS 2
#define ROOT_DIR_SIZE 14

#define ROOT_DIR_START (PARTITION_OFFSET + RESERVED_SECTORS + (NUM_FATS * FAT_SIZE))
#define DATA_REGION_START (ROOT_DIR_START + ROOT_DIR_SIZE)

uint8_t* fs_load_file(char* name, uint32_t* out_size) {
    uint8_t buf[512];
    
    // USE the macro here instead of hardcoded '2'
    disk_read(SLAVE_DRIVE, ROOT_DIR_START, 1, buf);

    DirectoryEntry* dir = (DirectoryEntry*)buf;

    for (int i = 0; i < 16; i++) {
        // Compare full 11 chars (NAME + EXT)
        if (memcmp(dir[i].name, name, 11) == 0) {
            *out_size = dir[i].size;
            
            uint32_t sectors = (dir[i].size + 511) / 512;
            
            // USE the dynamic Data Region start here
            uint32_t lba = DATA_REGION_START + (dir[i].cluster - 2);

            uint8_t* file_mem = malloc(sectors * 512 + 1);
            disk_read(SLAVE_DRIVE, lba, sectors, file_mem);
            
            file_mem[dir[i].size] = '\0'; 
            return file_mem;
        }
    }
    return 0;
}
