#include "fs.h"
#include "../stdio/stdio.h"
#include "../drivers/ata.h"

RAMFile ramdisk[MAX_FILES];


void init_ramdisk(){
    for(int i = 0; i < MAX_FILES; i++){
        ramdisk[i].active = 0;
    }
}

void create_file(char *name){
    for(int i = 0; i < MAX_FILES; i++){
        if(ramdisk[i].active == 0){
            strcpy(name, ramdisk[i].name);
            printf(0x07, "%s", ramdisk[i].name);
            ramdisk[i].content[0] = '\0';
            ramdisk[i].active = 1;

            printf(0x0A, "File '%s' created and saved to Sector %i\n", name, 100 + i);
            return;
        }
    }
    printf(0x0C, "%s\n", "Max files is reached (10).");
}

void write_file(char *file_name, char *content){
    for(int i = 0; i < MAX_FILES; i++){
        if(strcmp(file_name, ramdisk[i].name) == 'y' && ramdisk[i].active == 1){
            strcpy(content, ramdisk[i].content);
            printf(0x07, "%sg %sg\n", "successfully writen to", file_name);
            return;
        }
    }
    printf(0x0C, "%s doesn't exist.\n", file_name);
}

void read_file(char* file_name) {
    for(int i = 0; i < MAX_FILES; i++) {
        if(ramdisk[i].active && strcmp(ramdisk[i].name, file_name) == 'y') {
            printf(0x07, "%s\n", ramdisk[i].content);
            return;
        }
    }
    printf(0x0C, "%s\n", "Error: File not found");
}

void list_files(){
    for(int i = 0; i < MAX_FILES; i++){
        if(ramdisk[i].active == 0) continue;
        printf(0x07, "[%sW]\n", ramdisk[i].name);
    }
}

RAMFile blank;
void format(){
    printf(0x07, "Formatting Disk Sectors...\n");

    blank.active = 0;
    for(int i = 0; i < 32; i++) blank.name[i] = '\0';
    for(int i = 0; i < 479; i++) blank.content[i] = '\0';

    for(int s = 1; s <= 11; s++){
        printf(0x07, "Wiping Sector %i...\n", s);
    }

    printf(0x0A, "Disk is now clean.\n");
}
