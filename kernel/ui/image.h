#ifndef IMAGE_H
#define IMAGE_H

#include "../vga/vga.h"
#include "../libs/stdint.h"

void load_bmp(int x, int y, uint8_t* bmp_data);
void load_bmp_scaled(int start_x, int start_y, uint8_t* bmp_data, int target_w, int target_h);

#endif
