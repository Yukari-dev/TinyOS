#include "ata.h"
#include "../util/util.h"
#include "../stdio/stdio.h"


void scan_disk(){
    outb(0x1F6, 0xA0); // master
    outb(0x1F7, 0xEC); // command to identify
    
    unsigned char status = inb(0x1F7);

    if(status == 0){
        printf(0x07, "No disk detected on primary master.\n");
    } else{
        printf(0x02, "Disk detected!\n");
        for(int i = 0; i < 256; i++){
            unsigned short data = inw(0x1F0);
            if(i >= 27 && i <= 46){
                printc((char)(data >> 8), 0x07);
                printc((char)(data & 0xFF), 0x07);
            }

        }
        printc('\n', 0x07);
    }
}

void read_disk(unsigned int lba, unsigned short *buffer, unsigned char drive){
    unsigned char drive_bit = (drive == 0) ? 0xE0 : 0xF0;
    outb(0x1F6, (drive_bit | ((lba >> 24) & 0x0F)));
    outb(0x1F2, 1);
    outb(0x1F3, (unsigned short)lba);
    outb(0x1F4, (unsigned short)(lba >> 8));
    outb(0x1F5, (unsigned short)(lba >> 16));

    outb(0x1F7, 0x20);

    while(!(inb(0x1F7) & 0x08));

    for(int i = 0; i < 256; i++){
        buffer[i] = inw(0x1F0);
    }
    if(buffer[255] == 0xAA55){
        printf(0x02, "Valid Boot Sector Found!\n");
    } else if (buffer[255] != 0xAA55){
        printf(0x04, "Invalid Signature: %c\n", buffer[255]);
    }
}

void explore_fat_storage(){
    unsigned short bpd_buffer[256];

    read_disk(0, bpd_buffer, 1);
    unsigned char *ptr = (unsigned char*)bpd_buffer;

    unsigned short reserved = ptr[14] | (ptr[15] << 8);
    unsigned char fats = ptr[16];
    unsigned int fat_size = ptr[36] | (ptr[37] << 8) | (ptr[38] << 16) | (ptr[39] << 24);
    unsigned int data_lba = reserved + (fats * fat_size);
    printf(0x07, "Reserved: %i, FATs: %i, FAT size: %i\n", reserved, fats, fat_size);
    printf(0x02, "Files live at LBA: %i\n", data_lba);
}
void debug_drives() {
    // Check Master
    outb(0x1F6, 0xA0);
    unsigned char s0 = inb(0x1F7);
    printf(0x07, "Master Status: 0x%i ", s0);

    // Check Slave (The FAT drive)
    outb(0x1F6, 0xB0);
    unsigned char s1 = inb(0x1F7);
    printf(0x07, "Slave Status: 0x%i\n", s1);
}
