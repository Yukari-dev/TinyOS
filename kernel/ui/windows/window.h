#ifndef WINDOW_H
#define WINDOW_H
#include "../../common.h"
#include "../../drivers/mouse.h"
#include "../../vga/vga.h"
#include "../fonts/font.h"
#include "../apps/app.h"

#define MAX_WINDOWS 16

// Forward declare Window first
typedef struct Window Window;

// Define Window
struct Window {
    int x, y;
    int w, h;
    char* title;
    uint8_t color;
    bool is_dragging;
    bool is_resizing;
    int drag_offset_x, drag_offset_y;
    bool is_minimized;
    bool is_active;
    AppInterface* app;
};

// Now WindowManager can reference Window
typedef struct{
    Window pool[MAX_WINDOWS];
    int z_order[MAX_WINDOWS];
    int z_count;
} WindowManager;

extern WindowManager WM;

void wm_init();
int wm_create(int x, int y, int w, int h, char* title, uint8_t color);
void wm_update();
void wm_render();

#endif
