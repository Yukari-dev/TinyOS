#include "fs.h"
#include "../stdio/stdio.h"

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
            strcpy('\0', ramdisk[i].content);
            ramdisk[i].active = 1;
            printf(0x07, "%s: %sg\n", "File created", ramdisk[i].name);
            return;
        }
    }
    printf(0x07, "%s\n", "Max files is reached (10).");
}

void write_file(char *file_name, char *content){
    for(int i = 0; i < MAX_FILES; i++){
        if(strcmp(file_name, ramdisk[i].name) == 'y' && ramdisk[i].active == 1){
            strcpy(content, ramdisk[i].content);
            printf(0x07, "%sg %sg\n", "successfully writen to", file_name);
            return;
        }
    }
    printf(0x07, "%sb doesn't exist.", file_name);
}

char* read_file(char* file_name) {
    for(int i = 0; i < MAX_FILES; i++) {
        if(ramdisk[i].active && strcmp(ramdisk[i].name, file_name) == 'y') {
            return ramdisk[i].content;
        }
    }
    return "Error: File not found.";
}

void list_files(){
    for(int i = 0; i < MAX_FILES; i++){
        if(ramdisk[i].active == 0) continue;
        printf(0x07, "[%sW]\n", ramdisk[i].name);
    }
}


