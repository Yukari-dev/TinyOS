#include "font.h"
#include "font_data.h"
#include "../../vga/vga.h"

void draw_char(int x, int y, char c, unsigned char color){
    unsigned char* gylph = vga_font + (c * 8);
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            unsigned char mask = (1 << (7 - j));
            if(gylph[i] & mask){
                Screen.put_pixel(x + j, y + i, color);
            }
        }
    }
}

void draw_string(int x, int y, const char* string, unsigned char color){
    int offset = 0;
    for(int i = 0; string[i] != '\0'; i++){
        draw_char(x + offset, y, string[i], color);
        
        offset += 8;
    }
}

FontEngine Font = {
    .data = vga_font,
    .width = 8,
    .height = 8,
    .draw_char = draw_char,
    .draw_string = draw_string
};


