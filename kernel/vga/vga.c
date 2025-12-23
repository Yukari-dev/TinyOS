#include "vga.h"
#include "../util/util.h"


unsigned short* video_mem = (unsigned short*)0xB8000;
unsigned short width = 80;
unsigned short height = 25;
unsigned short cursor_y = 0;
unsigned short cursor_x = 0;


void disable_blinking() {
    inb(0x3DA); 
    outb(0x3C0, 0x30); 
    
    unsigned char reg = inb(0x3C1);
    reg &= ~0x08;     

    outb(0x3C0, reg);
}

void update_cursor(){
    unsigned short pos = cursor_y * width + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void enable_cursor(unsigned char cursor_start, unsigned char cursor_end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

