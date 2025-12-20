#ifndef SHELL_H
#define SHELL_C

extern char* commands[];

void initialize();

char str_cmp(unsigned char* str1, unsigned char* str2);
void execute_cmd(unsigned char* str1);

void clear_cmd();

void help_cmd();

void shut_down();

void reboot();

#endif
