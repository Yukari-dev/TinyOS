#include "drivers/mouse.h"
#include "vga/vga.h"
#include "ui/fonts/font.h"
#include "util/util.h"
#include "ui/windows/window.h"
#include "ui/apps/app.h"
#include "ui/taskbar.h"

extern AppInterface MonitorApp;

void main(){
    vga_init();
    
    while (1){
        vga_fill(0xFFFFFF);
        vga_draw_rect(0, 0, 100, 100, 0xFF0000);
        vga_flip();
    }
}
