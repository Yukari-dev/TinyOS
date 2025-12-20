#include "shell.h"
#include "../stdio/stdio.h"
#include "../util/util.h"
#include "../vga/vga.h"

char* commands[4] = {
    "clear", "help", "shutdown", "reboot"
};

void initialize(){
    printf(4, "%sr", ">");
    update_cursor();
}

char str_cmp(unsigned char *str1, unsigned char *str2){
    while(*str1 != '\0' && *str2 != 0){
        if(*str1 != *str2){
            return 'n';
        }
        *str1++;
        *str2++;
    }
    return 'y';
}

void execute_cmd(unsigned char *str1){
    for(int i = 0; i < 4; i++){
        unsigned char* target = commands[i];
        unsigned short match = 1;
        unsigned short j = 0;

        while(str1[j] != '\0' || target[j] != '\0'){
            if(str1[j] != target[j]){
                match = 0;
                break;
            }
            j++;
        }
        if(match){
            switch(i){
                case 0: clear_cmd(); break;
                case 1: help_cmd();  break;
                case 2: shut_down(); break;
                case 3: reboot();  break;
            }
            printf(0x07, "\n");
            initialize();
            return;
        }
    }
    printf(0x0F, "'%sw'unknown command.", str1);
    printf(0x07, "\n");
    initialize();
}


void clear_cmd(){
    clear();
}

void help_cmd(){
    
}

void shut_down(){
    printf(0x04, "\nShutting down...");
    outw(0x604, 0x2000);
    printf(0x0F, "\nIt is now safe to turn off your computer.");
    __asm__ __volatile__("cli");
    while(1){
        __asm__ __volatile__("hlt");
    }

}

void reboot(){
    printf(0x0E, "\nRebooting system...");
    unsigned short good = 0x02;
    while(good & 0x02){
        good = inb(0x64);
    }

    outb(0x64, 0xFE);
    __asm__ __volatile__("hlt");
}


