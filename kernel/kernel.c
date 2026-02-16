#include "drivers/mouse.h"
#include "vga/vga.h"
#include "ui/fonts/font.h"
#include "util/util.h"
#include "ui/windows/window.h"

void main(){
    // Clear keyboard buffer
    while (inb(0x64) & 1) { inb(0x60); }
    
    Mouse.init();
    wm_init();
    int my_app = wm_create(100, 50, 80, 40, "Monitor", 0);

    while (1){
        Mouse.update();
        Screen.clear(55);
        wm_update();
        wm_render();

        
        unsigned char cursor_color = Mouse.left_clicked ? 4 : 15;
        Screen.draw_rect(Mouse.x, Mouse.y, 2, 2, cursor_color);

        Screen.flip();
    }
}
