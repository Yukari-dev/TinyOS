#include "app.h"
#include "../../libs/string.h"
#include "../../vga/vga.h"
#include "../../ui/fonts/font.h"
#include "../../drivers/mouse.h"

static char buf[16];

void monitor_draw(int x, int y, int w, int h) {
    int px = x + 5;
    int py = y + 5;

    Font.draw_string(px, py, "Mouse X:", 15);
    Font.draw_string(px + 65, py, itoa(Mouse.x, buf, 10), 10); // Green color (10)

    Font.draw_string(px, py + 15, "Mouse Y:", 15);
    Font.draw_string(px + 65, py + 15, itoa(Mouse.y, buf, 10), 10);

    vga_draw_rect(x + 2, py + 30, w - 4, 1, 8); // Dark grey line

    Font.draw_string(px, py + 40, "Status: Running", 14); // Yellow color (14)
}

AppInterface MonitorApp = {
    .name = "SysMonitor",
    .on_init = NULL,
    .on_update = NULL,
    .on_render = monitor_draw
};

