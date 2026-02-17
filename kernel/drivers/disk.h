#ifndef DISK_H
#define DISK_H

#include "../libs/stdint.h"

void disk_read(uint8_t drive, uint32_t lba, uint8_t count, uint8_t* target);

#endif
