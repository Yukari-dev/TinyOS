#include "shell.h"
#include "../stdio/stdio.h"
#include "../util/util.h"
#include "../vga/vga.h"
#include "../fs/fs.h"
#include "../drivers/ata.h"
#include "../time/time.h"
#define NULL ((void*)0)

Command command_table[] = {
    {"clear", "Clears the screen.", cmd_clear},
    {"echo", "output/input the arguments to the console.", cmd_echo},
    {"touch", "create a file", cmd_touch},
    {"cat", "read a file", cmd_cat},
    {"ls", "list all files", cmd_ls},
    {"lsdisk", "list all diskes", cmd_lsdisk},
    {"dump", "dump the active disk content", cmd_dump},
    {"format", "format the active disk content", cmd_format},
    {"help", "Show the help screen.", cmd_help},
    {"shutdown", "Shutdown the computer.", cmd_shutdown},
    {"reboot", "Reboot the computer.", cmd_reboot},

};

unsigned short num_commands = sizeof(command_table) / sizeof(Command);

void initialize(){
    printf(4, "%sr", ">");
    update_cursor();
}


void execute_cmd(unsigned char *input){
    char* arg = 0;
    
    for(int i = 0; input[i] != '\0'; i++){
        if(input[i] == ' '){
            input[i] = '\0';
            arg = &input[i + 1];
            break;
        }
    }

    for(int i = 0; i < num_commands; i++){
        if(strcmp(input, command_table[i].name) == 1){
            command_table[i].function(arg);
            initialize();
            return;
        }
    }
    printf(0x0F, "'%sW'unknown command.", input);
    printf(0x07, "\n");
    initialize();
}

void cmd_clear(char* arg){
    clear();
}

void cmd_echo(char* arg){
    if(!arg){
        printf(0x0C, "Usage: echo \"text\" >> file.txt\n");
        return;
    }

    char* content = strtok(arg, "\"");
    char* op = strtok(NULL, " ");
    char* file = strtok(NULL, " ");

    if (content && op && file && strcmp(op, ">>") == 'y') {
        write_file(file, content);
        return;
    }
    if (content) printf(0x07, "%s\n", content);
}

void cmd_touch(char* arg){
    if(arg){
        create_file(arg);
    } else{
        printf(0x07, "%sr", "you must have at least one argument to create a file.\n");
    }
}

void cmd_ls(char* arg){
    list_files();
}

void cmd_lsdisk(char* arg){
    printf(0x07, "[%so]Scanning for IDE/ATA devices...\n", "-");
    scan_drives();

    int found = 0;
    for(int i = 0; i < 4; i++){
        if(detected_drives[i].exists){
            printf(0x07, "Disk [%i]: %sW\n", i, detected_drives[i].model);
            found = 1;
        }
    }

    if(!found) printf(0x0C, "No drives Detected.\n");

}

void cmd_dump(char* arg){
    unsigned short disk_buffer[256]; // A local 512-byte buffer
    scan_drives();

    printf(0x07, "Reading Sector 1 (Directory Slot)...\n");

    ata_rw_sector(1, disk_buffer, 0x20); 

    unsigned char* bytes = (unsigned char*)disk_buffer;

    for(int i = 0; i < 64; i++) {
        printf(0x07, "%i", bytes[i]); 
        printc(' ', 0x07);

        if((i + 1) % 16 == 0) printf(0x07, "\n");
    }
}

void cmd_format(char* arg){
    format();
}

void cmd_cat(char* arg){
    if(arg){
        read_file(arg);
    } else{
        printf(0x0C, "%sr", "Usage: cat <filename>\n");
    }
}

void cmd_help(char* arg){
    for(int i = 0; i < num_commands; i++){
        printf(0x07, "%sW - %s\n", command_table[i].name, command_table[i].description);
    }
}

void cmd_shutdown(char* arg){
    printf(0x04, "\nShutting down...");
    outw(0x604, 0x2000);
    printf(0x0F, "\nIt is now safe to turn off your computer.");
    __asm__ __volatile__("cli");
    while(1){
        __asm__ __volatile__("hlt");
    }

}

void cmd_reboot(char* arg){
    printf(0x0E, "\nRebooting system...");
    unsigned short good = 0x02;
    while(good & 0x02){
        good = inb(0x64);
    }

    outb(0x64, 0xFE);
    __asm__ __volatile__("hlt");
}


