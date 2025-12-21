#include "ata.h"
#include "../util/util.h"
#include "../stdio/stdio.h"

DriveInfo detected_drives[4];

unsigned short active_port = 0;
unsigned char active_dev = 0;



void ata_rw_sector(unsigned int lba, unsigned short* buffer, unsigned char command){
    outb(active_port + 6, ((lba >> 24) & 0x0F) | active_dev);
    outb(active_port + 2, 1);                        
    outb(active_port + 3, (unsigned char)lba);    
    outb(active_port + 4, (unsigned char)(lba >> 8));
    outb(active_port + 5, (unsigned char)(lba >> 16));
    outb(active_port + 7, command);                

    while (inb(active_port + 7) & 0x80); 

    while (!(inb(active_port + 7) & 0x08)) {
        if (inb(active_port + 7) & 0x01) {
            printf(0x0C, "DISK ERROR!\n");
            return;
        }
    }

    if (command == 0x30) { 
        for (int i = 0; i < 256; i++) outw(active_port, buffer[i]);
    } else {
        for (int i = 0; i < 256; i++) buffer[i] = inw(active_port);
    }
    
    for(int i = 0; i < 4; i++) inb(active_port + 7);
}


void scan_drives() {
    unsigned short ports[2] = {0x1F0, 0x170};
    unsigned char drives[2] = {0xE0, 0xF0};
    int count = 0;

    for (int p = 0; p < 2; p++) {
        for (int d = 0; d < 2; d++) {
            unsigned short base = ports[p];
            unsigned char dev = drives[d];

            outb(base + 6, dev);
            outb(base + 7, 0xEC); 

            if (inb(base + 7) == 0) continue;

            while (inb(base + 7) & 0x80);
            if (!(inb(base + 7) & 0x08)) continue;

            unsigned short buffer[256];
            for (int i = 0; i < 256; i++) {
                buffer[i] = inw(base);
            }

            for (int i = 0; i < 20; i++) {
                detected_drives[count].model[i*2] = (char)(buffer[27 + i] >> 8);
                detected_drives[count].model[i*2 + 1] = (char)(buffer[27 + i] & 0xFF);
            }
            detected_drives[count].model[40] = '\0';
            detected_drives[count].exists = 1;

            set_active_drive(base, dev);

            unsigned int sectors = *((unsigned int*)&buffer[60]);
            unsigned int size_mb = (sectors * 512) / 1024 / 1024;
            
            printf(0x0A, "Found: %s at Port %i\n", detected_drives[count].model, base);
            printf(0x0E, "Capacity: %i MB (%i sectors)\n", size_mb, sectors);
            count++;
        }
    }
}

void set_active_drive(unsigned short port, unsigned char dev){
    active_port = port;
    active_dev = dev;
}


