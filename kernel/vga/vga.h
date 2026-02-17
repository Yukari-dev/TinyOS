#ifndef VGA_H
#define VGA_H

#include "../libs/stdint.h"

typedef struct {
    uint32_t* lfb;    
    uint32_t* buffer; 
    int width;
    int height;
    int pitch;
    uint8_t bpp;
} ScreenDevice;

extern ScreenDevice Screen;

void vga_init();
void vga_put_pixel(int x, int y, uint32_t color);
void vga_fill(uint32_t color);
void vga_draw_rect(int x, int y, int w, int h, uint32_t color);
void vga_flip();

#endif
