#include "window.h"

WindowManager WM;

void wm_init(){
    for(int i = 0; i < MAX_WINDOWS; i++) {
        WM.pool[i].is_active = false;
        WM.z_order[i] = i;
    }
    WM.z_count = 0;
}

int wm_create(int x, int y, int w, int h, char* title, unsigned char color){
    for(int i = 0; i < MAX_WINDOWS; i++){
        if(!WM.pool[i].is_active){
            Window* win = &WM.pool[i];
            memset(win, 0, sizeof(Window));
            win->x = x;
            win->y = y;
            win->w = w;
            win->h = h;
            win->title = title;
            win->color = color;
            win->is_active = true;
            WM.z_order[WM.z_count++] = i;

            return i;
        }
    }
    return -1;
}


void wm_bring_to_front(int window_index){
    int pos = -1;
    for(int i = 0; i < WM.z_count; i++){
        if(WM.z_order[i] == window_index){
            pos = i;
            break;
        }
    }

    if(pos == -1) return;

    for(int i = pos; i < WM.z_count; i++){
        WM.z_order[i] = WM.z_order[i + 1];
    }

    WM.z_order[WM.z_count - 1] = window_index;
}


void wm_update(){
    for(int z = WM.z_count - 1; z >= 0; z--){
        int i = WM.z_order[z];
        Window* win = &WM.pool[i];
        if(!win->is_active) continue;
        
        if(Mouse.left_clicked){
            if(Mouse.x >= win->x + win->w - 10 && Mouse.x <= win->x + win->w &&
               Mouse.y >= win->y + win->h - 10 && Mouse.y <= win->y + win->h){
                win->is_resizing = true;
                win->drag_offset_x = Mouse.x;
                win->drag_offset_y = Mouse.y;
                wm_bring_to_front(i);
                break;  // Only interact with topmost window
            }
            else if(Mouse.x >= win->x && Mouse.x <= win->x + win->w &&
                    Mouse.y >= win->y && Mouse.y <= win->y + 12) 
            {
                win->is_dragging = true;
                win->drag_offset_x = Mouse.x;
                win->drag_offset_y = Mouse.y;
                wm_bring_to_front(i);
                break;
            } else if(Mouse.x >= win->x && Mouse.x < win-> x + win->w &&
                      Mouse.y >= win->y && Mouse.y <= win->y + win->h){
                wm_bring_to_front(i);
            }
        }
    }
    
    // Update all windows
    for(int i = 0; i < MAX_WINDOWS; i++){
        Window* win = &WM.pool[i];
        if(!win->is_active) continue;
        
        if(!Mouse.left_down){
            win->is_dragging = false;
            win->is_resizing = false;
        }

        if(win->is_dragging){
            win->x += Mouse.x - win->drag_offset_x;
            win->y += Mouse.y - win->drag_offset_y;
            win->drag_offset_x = Mouse.x;
            win->drag_offset_y = Mouse.y;
        }

        if(win->is_resizing){
            int delta_x = Mouse.x - win->drag_offset_x;
            int delta_y = Mouse.y - win->drag_offset_y;
            win->w += delta_x;
            win->h += delta_y;

            if(win->w < 100) win->w = 100;
            if(win->h < 50) win->h = 50;

            win->drag_offset_x = Mouse.x;
            win->drag_offset_y = Mouse.y;
        }
    }
}

void wm_render(){
    for(int z = 0; z < WM.z_count; z++){
        int i = WM.z_order[z];
        Window* win = &WM.pool[i];
        if(!win->is_active) continue;
        
        // Body
        Screen.draw_rect(win->x, win->y, win->w, win->h, win->color);    

        // TitleBar
        Screen.draw_rect(win->x, win->y, win->w, 12, 8);
        Font.draw_string(win->x + 5, win->y + 2, win->title, 15);

        if(win->app != NULL && win->app->on_render != NULL){
            win->app->on_render(win->x, win->y + 12, win->w, win->h - 12);
        }

    }
}

