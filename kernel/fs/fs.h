#ifndef FS_H
#define FS_H

#define MAX_FILES 10
#define MAX_FILE_SIZE 512

typedef struct{
    char name[32];
    char content[MAX_FILE_SIZE];
    unsigned int size;
    unsigned int active;
}RAMFile;

extern RAMFile ramdisk[MAX_FILES];

void init_ramdisk();

void create_file(char* name);

void write_file(char* file_name, char* content);

char* read_file(char* file_name);

void list_files();

#endif
