#include "window.h"

WindowManager WM;

void wm_init(){
    for(int i = 0; i < MAX_WINDOWS; i++) WM.pool[i].is_active = false;
}

int wm_create(int x, int y, int w, int h, char* title, unsigned char color){
    for(int i = 0; i < MAX_WINDOWS; i++){
        if(!WM.pool[i].is_active){
            WM.pool[i] = (Window){x, y, w, h, title, color, false, 0, 0, false, true, NULL};
            return i;
        }
    }
    return -1;
}

void wm_update(){
    for(int i = 0; i < MAX_WINDOWS; i++){
        Window* win = &WM.pool[i];
        if(!win->is_active) continue;
        if(Mouse.left_clicked){
            if(Mouse.x >= win->x && Mouse.x <= win->x + win->w &&
               Mouse.y >= win->y && Mouse.y <= win->y + 12) 
            {
                win->is_dragging = true;
                win->drag_offset_x = Mouse.x;
                win->drag_offset_y = Mouse.y;
                WM.top_window_index = i;
                // Simple Z-order: we could swap array positions here 
                // to bring 'win' to index MAX_WINDOWS - 1
            }
        }
        if(!Mouse.left_down)
            win->is_dragging = false;

        if(win->is_dragging){
            win->x += Mouse.x - win->drag_offset_x;
            win->y += Mouse.y - win->drag_offset_y;
            win->drag_offset_x = Mouse.x;
            win->drag_offset_y = Mouse.y;
        }
    }
}

void wm_render(){
    for(int i = 0; i < MAX_WINDOWS; i++){
        Window* win = &WM.pool[i];
        if(!win->is_active) continue;
        
        // Body
        Screen.draw_rect(win->x, win->y, win->w, win->h, win->color);    

        // TitleBar
        Screen.draw_rect(win->x, win->y, win->w, 12, 8);
        Font.draw_string(win->x + 5, win->y + 2, win->title, 15);
    }
}

