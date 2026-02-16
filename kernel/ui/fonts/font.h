#ifndef FONT_H
#define FONT_H

typedef struct{
    unsigned char* data;
    int width;
    int height;

    void (*draw_char)(int x, int y, char c, unsigned char color);
    void (*draw_string)(int x, int y, const char* str, unsigned char color);
} FontEngine;

extern FontEngine font;

#endif
