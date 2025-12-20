#include "vga/vga.h"
#include "stdio/stdio.h"


void main(){
    clear();
    printf(0x07, "%sgWelcome to %sr %iw!\n", "[>]", "T-OS", 24);
    for(int i = 0; i < 25; i++){
        printf(0x07, "%i\n", i);
    }

    cursor_y = 1;
    update_cursor();
    scroll();

    while(1);
}

