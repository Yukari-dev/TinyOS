#ifndef MOUSE_H
#define MOUSE_H

#include "../util/util.h"

#define MOUSE_WIDTH 800
#define MOUSE_HEIGHT 600

typedef struct {
    int x;
    int y;
    unsigned char left_clicked;
    unsigned char left_down;
    unsigned char cycle;
    unsigned char packet[3];

    void (*init)();
    void (*update)();
} MouseDevice;

extern MouseDevice Mouse;

#endif
