#include "vga.h"
#include "../util/util.h"

unsigned char back_buffer[SIZE];

void clear(unsigned char color){
    for(int i = 0; i < SIZE; i++) back_buffer[i] = color;
}

void put_pixel(int x, int y, unsigned char color){
    back_buffer[y * WIDTH + x] = color;
}

void draw_rect(int x, int y, int w, int h, unsigned char color){
    for(int i = 0; i < h; i++){
        for(int j = 0; j < w; j++){
            back_buffer[(y + i) * WIDTH + (x + j)] = color;
        }
    }
}

void flip(){
    unsigned char* vram = (unsigned char*)0xA0000;
    for(int i = 0; i < SIZE; i++) vram[i] = back_buffer[i];
}
VGADriver Screen = {
    .buffer = back_buffer,
    .clear = clear,
    .put_pixel = put_pixel,
    .draw_rect = draw_rect,
    .flip = flip
};
