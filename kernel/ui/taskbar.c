#include "taskbar.h"

void taskbar_render() {
    // 1. Draw Taskbar Background (Bottom 16 pixels)
    // VGA Height is 200, so 184 to 200
    Screen.draw_rect(0, 184, 320, 16, 7); // Light grey

    int current_x = 5;
    for (int i = 0; i < MAX_WINDOWS; i++) {
        Window* win = &WM.pool[i];
        if (win->is_active) {
            // Draw a small "button" for each window
            Screen.draw_rect(current_x, 186, 60, 12, 8); // Darker grey
            
            // Draw the first 7 letters of the window title
            Font.draw_string(current_x + 2, 188, win->title, 15);
            
            current_x += 65; // Move to next slot
        }
    }
}
