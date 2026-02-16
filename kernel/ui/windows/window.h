#ifndef WINDOW_H
#define WINDOW_H
#include "../../common.h"
#include "../../drivers/mouse.h"
#include "../../vga/vga.h"
#include "../fonts/font.h"
#include "../app.h"

#define MAX_WINDOWS 16

typedef struct{
    int x, y;
    int w, h;
    char* title;
    uint8_t color;
    bool is_dragging;
    int drag_offset_x, drag_offset_y;
    bool is_minimized;
    bool is_active;
    AppInterface* runningApp;
} Window;

typedef struct{
    Window pool[MAX_WINDOWS];
    int top_window_index;
} WindowManager;

extern WindowManager WM;

void wm_init();
int wm_create(int x, int y, int w, int h, char* title, uint8_t color);
void wm_update();
void wm_render();

#endif
