#ifndef SHELL_H
#define SHELL_C

typedef void (*command_func)(char* arg);

typedef struct{
    char* name;
    char* description;
    command_func function;
} Command;

extern unsigned short num_commands;

void initialize();

char str_cmp(unsigned char* str1, unsigned char* str2);
void execute_cmd(unsigned char* str1);


// COMMANDS
void cmd_clear(char* arg);

void cmd_echo(char* arg);

void cmd_create_file(char* arg);

void cmd_list_file(char* arg);

void cmd_help(char* arg);

void cmd_shutdown(char* arg);

void cmd_reboot(char* arg);

#endif
