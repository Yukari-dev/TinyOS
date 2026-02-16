#ifndef VGA_H
#define VGA_H

#define WIDTH 320
#define HEIGHT 200
#define SIZE WIDTH*HEIGHT

typedef struct{
    unsigned char* buffer;
    void (*clear)(unsigned char color);
    void (*put_pixel)(int x, int y, unsigned char color);
    void (*draw_rect)(int x, int y, int w, int h, unsigned char color);
    void (*flip)();
} VGADriver;

extern VGADriver Screen;

#endif
