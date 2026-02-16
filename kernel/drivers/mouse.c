#include "mouse.h"

static void mouse_wait(unsigned char type) {
    unsigned int timeout = 100000;
    while (timeout--) {
        if (type == 0 && (inb(0x64) & 1)) return;
        if (type == 1 && !(inb(0x64) & 2)) return;
    }
}

static void mouse_write(unsigned char a) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, a);
}

void init_mouse() {
    unsigned char status;
    mouse_wait(1);
    outb(0x64, 0xA8);
    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    status = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);
    mouse_write(0xF4);
    inb(0x60); // Acknowledge
}

void update_mouse() {
    unsigned char status = inb(0x64);
    if ((status & 0x01) && (status & 0x20)) {
        unsigned char data = inb(0x60);
        if (Mouse.cycle == 0 && !(data & 0x08)) return;

        Mouse.packet[Mouse.cycle++] = data;

        if (Mouse.cycle == 3) {
            Mouse.cycle = 0;
            Mouse.left_clicked = (Mouse.packet[0] & 0x01);
            Mouse.x += (signed char)Mouse.packet[1];
            Mouse.y -= (signed char)Mouse.packet[2];

            if (Mouse.x < 0) Mouse.x = 0;
            if (Mouse.y < 0) Mouse.y = 0;
            if (Mouse.x >= MOUSE_WIDTH) Mouse.x = MOUSE_WIDTH - 1;
            if (Mouse.y >= MOUSE_HEIGHT) Mouse.y = MOUSE_HEIGHT - 1;
        }
    } else if (status & 0x01) {
        inb(0x60); // Drain keyboard
    }
}

// Instantiate the "Object"
MouseDevice Mouse = {
    .x = 160,
    .y = 100,
    .left_clicked = 0,
    .cycle = 0,
    .init = init_mouse,
    .update = update_mouse
};
