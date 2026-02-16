#ifndef MATH_H
#define MATH_H
#include "stdbool.h"

typedef struct{
    int x, y, w, h;
} Rect;

static inline bool point_in_rect(int px, int py, Rect rect){
    return (px >= rect.x && px <= rect.x + rect.w && py >= rect.y && py <= rect.y + rect.h);
}

static inline bool rect_overlapping(Rect a, Rect b){
    return (
        a.x < b.x + b.w && a.x + a.w > b.x &&
        a.y < b.y + b.h && a.y + a.h > b.y);
}

#endif
