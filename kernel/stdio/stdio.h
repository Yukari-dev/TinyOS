#ifndef STDIO_H
#define STDIO_H

typedef struct {
    char code;
    unsigned char color_val;
} ColorMap;

enum TextType{
    NORMAL,
    PROGRESS,
    DONE,
    INFO,
    WARNING,
    ERROR
};

void printf(const char color, const char* format, ...);
void printc(char c, const char color);
void print_at(short x, short y, char* str, unsigned char color);

char* num_to_char(int num);

unsigned char find_color(char c);

short len(char* c);

void strcpy(const char* source, char* destination);

char* strtok(char* input_string, const char* delimiter);

char strcmp(unsigned char *str1, unsigned char *str2);

void clear();

void scroll();

#endif
