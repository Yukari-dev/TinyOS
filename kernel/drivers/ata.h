#ifndef ATA_H
#define ATA_H

void scan_disk();

void read_disk(unsigned int lba, unsigned short *buffer, unsigned char drive);

void explore_fat_storage();

void debug_drives();

#endif
