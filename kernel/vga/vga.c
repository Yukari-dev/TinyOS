#include "vga.h"

#define video_mem 0xB8000


void outb(unsigned short port, unsigned char data){
    __asm__("outb %0, %1" : : "a"(data), "Nd"(port));
}

void update_cursor(){
    unsigned short pos = cursor_y * width + cursor_x;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(pos & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}


