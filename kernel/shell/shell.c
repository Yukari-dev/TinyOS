#include "shell.h"
#include "../stdio/stdio.h"
#include "../util/util.h"
#include "../vga/vga.h"

Command command_table[] = {
    {"clear", "Clears the screen.", cmd_clear},
    {"echo", "output the arguments to the console.", cmd_echo},
    {"help", "Show the help screen.", cmd_help},
    {"shutdown", "Shutdown the computer.", cmd_shutdown},
    {"reboot", "Reboot the computer.", cmd_reboot},

};

unsigned short num_commands = sizeof(command_table) / sizeof(Command);

void initialize(){
    printf(4, "%sr", ">");
    update_cursor();
}

char str_cmp(unsigned char *str1, unsigned char *str2){
    unsigned short j = 0;
    while(str1[j] != '\0' || str2[j] != 0){
        if(str1[j] != str2[j]){
            return 'n';
        }
        j++;
    }
    return 'y';
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
        if(str_cmp(input, command_table[i].name) == 'y'){
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
    if(arg){
        printf(0x0F, "%s\n", arg);
    } else{
        printf(0x07, "%sr", "you must have at least one argument to output.\n");
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


