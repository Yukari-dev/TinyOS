#include "vga.h"
#include "../libs/string.h"

uint8_t back_buffer[SIZE];

void clear(uint8_t color){
    memset(back_buffer, color, SIZE);
}

void put_pixel(int x, int y, uint8_t color){
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
    back_buffer[y * WIDTH + x] = color;
}

void draw_rect(int x, int y, int w, int h, uint8_t color) {
    for(int i = 0; i < h; i++) {
        int screen_y = y + i;

        if(screen_y < 0 || screen_y >= HEIGHT) continue;
        
        int start_x = x;
        int draw_w = w;
        if(start_x < 0){
            draw_w += start_x;
            start_x = 0;
        }

        if(start_x + draw_w > WIDTH){
            draw_w = WIDTH - start_x;
        }
        if(draw_w > 0)
            memset(&back_buffer[screen_y * WIDTH + start_x], color, draw_w);
    }
}

void flip() {
    uint8_t* vram = (uint8_t*)0xA0000;
    memcpy(vram, back_buffer, SIZE);
}

VGADriver Screen = {
    .buffer = back_buffer,
    .clear = clear,
    .put_pixel = put_pixel,
    .draw_rect = draw_rect,
    .flip = flip
};
