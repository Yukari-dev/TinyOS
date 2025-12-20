#include "vga/vga.h"
#include "stdio/stdio.h"


void main(){
    clear();
    printf(0x07, "%sgWelcome to %sr %iw!\n", "[>]", "T-OS", 24);

    while(1);
}

