#include "drivers/mouse.h"
#include "vga/vga.h"
#include "ui/fonts/font.h"
#include "util/util.h"
#include "ui/windows/window.h"
#include "ui/apps/app.h"

extern AppInterface MonitorApp;

void main(){
    // Clear keyboard buffer
    while (inb(0x64) & 1) { inb(0x60); }
    
    Mouse.init();
    wm_init();
    int win_id = wm_create(100, 50, 150, 80, "Monitor", 1);
    int win_id2 = wm_create(100, 50, 150, 80, "Monitor2", 1);
    WM.pool[win_id].app = &MonitorApp;
    WM.pool[win_id2].app = &MonitorApp;

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
