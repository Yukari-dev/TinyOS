#ifndef ATA_H
#define ATA_H

typedef struct{
    unsigned char port;
    unsigned char drive;
    char model[41];
    int exists;
} DriveInfo;

extern DriveInfo detected_drives[4];

void ata_rw_sector(unsigned int lba, unsigned short* buffer, unsigned char command);

void scan_drives();

void set_active_drive(unsigned short port, unsigned char dev);

#endif
