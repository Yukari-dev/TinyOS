#include "drivers/mouse.h"
#include "vga/vga.h"


void main(){
    while (inb(0x64) & 1) { inb(0x60); }
    Mouse.init();
    while (1){
        Mouse.update();
        Screen.clear(56);

        unsigned char cursor_color = Mouse.left_clicked ? 4 : 15;
        Screen.draw_rect(Mouse.x, Mouse.y, 3, 3, cursor_color);

        Screen.flip();
    }
}

