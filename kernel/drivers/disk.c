#include "disk.h"
#include "../util/util.h"

void disk_read(uint8_t drive, uint32_t lba, uint8_t count, uint8_t* target) {
    // 0xE0 = Master, 0xF0 = Slave
    uint8_t drive_select = (drive == 0) ? 0xE0 : 0xF0;

    outb(0x1F6, (lba >> 24) | drive_select);
    outb(0x1F2, count);
    outb(0x1F3, (uint8_t)lba);
    outb(0x1F4, (uint8_t)(lba >> 8));
    outb(0x1F5, (uint8_t)(lba >> 16));
    outb(0x1F7, 0x20); // Read Command

    for (int j = 0; j < count; j++) {
        while (!(inb(0x1F7) & 0x08));
        uint16_t* ptr = (uint16_t*)target + (j * 256);
        for (int i = 0; i < 256; i++) {
            ptr[i] = inw(0x1F0);
        }
    }
}
