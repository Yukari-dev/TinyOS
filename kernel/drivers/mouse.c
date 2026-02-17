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
    // Keep reading as long as there is data in the buffer (status bit 0 is set)
    while (inb(0x64) & 0x01) {
        unsigned char status = inb(0x64);
        
        // Ensure the data is actually from the mouse (bit 5)
        if (status & 0x20) {
            unsigned char data = inb(0x60);
            if (Mouse.cycle == 0 && !(data & 0x08)) continue;

            Mouse.packet[Mouse.cycle++] = data;

            if (Mouse.cycle == 3) {
                Mouse.cycle = 0;
                unsigned char button_state = (Mouse.packet[0] & 0x01);
                Mouse.left_clicked = (button_state && !Mouse.left_down);
                Mouse.left_down = button_state;
                
                Mouse.x += (signed char)Mouse.packet[1];
                Mouse.y -= (signed char)Mouse.packet[2];

                if (Mouse.x < 0) Mouse.x = 0;
                if (Mouse.y < 0) Mouse.y = 0;
                if (Mouse.x >= 800) Mouse.x = 799;
                if (Mouse.y >= 600) Mouse.y = 599;
            }
        } else {
            inb(0x60);
        }
    }
}

// Instantiate the "Object"
MouseDevice Mouse = {
    .x = 400,
    .y = 300,
    .left_clicked = 0,
    .left_down = 0,
    .cycle = 0,
    .init = init_mouse,
    .update = update_mouse
};
