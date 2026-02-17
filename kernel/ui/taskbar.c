#include "taskbar.h"

void taskbar_render() {
    vga_draw_rect(0, 184, 320, 16, 0xF0F1F3);
    int current_x = 5;
    for (int i = 0; i < MAX_WINDOWS; i++) {
        Window* win = &WM.pool[i];
        if (win->is_active) {
            vga_draw_rect(current_x, 186, 60, 12, 0xFFFFFF);            
            Font.draw_string(current_x + 2, 188, win->title, 0x000000);
            
            current_x += 65;
        }
    }
}
