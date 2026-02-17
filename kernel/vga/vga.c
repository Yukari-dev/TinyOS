#include "vga.h"

#define VBE_MODE_INFO 0x9000
#define PITCH_OFFSET 16
#define BPP_OFFSET 25 
#define LFB_POINTER_OFFSET 40

ScreenDevice Screen;

void vga_init() {
    Screen.pitch = *(uint16_t*)(VBE_MODE_INFO + PITCH_OFFSET);
    Screen.bpp = *(uint8_t*)(VBE_MODE_INFO + BPP_OFFSET);
    
    uint32_t* phys_lfb = (uint32_t*)(*(uint32_t*)(VBE_MODE_INFO + LFB_POINTER_OFFSET));

    Screen.width = 800;
    Screen.height = 600;
    Screen.lfb = phys_lfb;
    Screen.buffer = (uint32_t*)0x2000000; 
}

void vga_flip() {
    if (Screen.bpp == 32) {
        for(int y = 0; y < Screen.height; y++){
            uint32_t* dest = (uint32_t*)((uint8_t*)Screen.lfb + (y * Screen.pitch));
            uint32_t* src = Screen.buffer + (y * Screen.width);

            for(int x = 0; x < Screen.width; x++){
                dest[x] = src[x];
            }
        }
    } 
    else if (Screen.bpp == 24) {
        for(int y = 0; y < Screen.height; y++){
            uint8_t* dest = (uint8_t*)Screen.lfb + (y * Screen.pitch);
            uint32_t* src = Screen.buffer + (y * Screen.width);
            
            for(int x = 0; x < Screen.width; x++){
                uint32_t c = src[x];
                *dest++ = (c & 0xFF);         // Blue
                *dest++ = (c >> 8) & 0xFF;    // Green
                *dest++ = (c >> 16) & 0xFF;   // Red
            }
        }
    }
}

void vga_put_pixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= Screen.width || y < 0 || y >= Screen.height) return;
    Screen.buffer[y * Screen.width + x] = color;
}

void vga_draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            vga_put_pixel(x + j, y + i, color);
        }
    }
}

void vga_draw_gradient(uint32_t color1, uint32_t color2) {
    uint8_t r1 = (color1 >> 16) & 0xFF;
    uint8_t g1 = (color1 >> 8) & 0xFF;
    uint8_t b1 = color1 & 0xFF;

    uint8_t r2 = (color2 >> 16) & 0xFF;
    uint8_t g2 = (color2 >> 8) & 0xFF;
    uint8_t b2 = color2 & 0xFF;

    for (int y = 0; y < Screen.height; y++) {
        int p = (y * 255) / Screen.height;

        uint8_t r = r1 + (p * (r2 - r1)) / 255;
        uint8_t g = g1 + (p * (g2 - g1)) / 255;
        uint8_t b = b1 + (p * (b2 - b1)) / 255;

        uint32_t final_color = (r << 16) | (g << 8) | b;

        for (int x = 0; x < Screen.width; x++) {
            Screen.buffer[y * Screen.width + x] = final_color;
        }
    }
}

void vga_fill(uint32_t color) {
    for (int i = 0; i < Screen.width * Screen.height; i++) {
        Screen.buffer[i] = color;
    }
}

