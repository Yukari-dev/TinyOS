#ifndef STDIO_H
#define STDIO_H

typedef struct {
    char code;
    unsigned char color_val;
} ColorMap;

void printf(const char color, const char* format, ...);
void printc(char c, const char color);

char* num_to_char(int num);

unsigned char find_color(char c);

short len(char* c);

void clear();

void scroll();

#endif
