#include "image.h"

void load_bmp(int px, int py, uint8_t* bmp_data){
    uint32_t data_offset = *(uint32_t*)&bmp_data[10];
    int width = *(int*)&bmp_data[18];
    int height = *(int*)&bmp_data[22];
    
    uint8_t* pixels = bmp_data + data_offset;

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int pixel_index = ((height - 1 - y) * width + x) * 3;
            uint8_t red = pixels[pixel_index];
            uint8_t green = pixels[pixel_index + 1];
            uint8_t blue = pixels[pixel_index + 2];

            uint32_t color = (red << 16) | (green << 8) | blue;
            vga_put_pixel(px+x, py+y, color);
        }
    }

}

void load_bmp_scaled(int start_x, int start_y, uint8_t* bmp_data, int target_w, int target_h) {
    uint32_t data_offset = *(uint32_t*)&bmp_data[10];
    int orig_w = *(int*)&bmp_data[18];
    int orig_h = *(int*)&bmp_data[22];
    uint8_t* pixels = bmp_data + data_offset;

    uint32_t x_step = (orig_w << 16) / target_w;
    uint32_t y_step = (orig_h << 16) / target_h;

    for (int y = 0; y < target_h; y++) {
        for (int x = 0; x < target_w; x++) {
            int cur_orig_x = (x * x_step) >> 16;
            int cur_orig_y = (y * y_step) >> 16;

            int flipped_y = (orig_h - 1 - cur_orig_y);
            
            int pixel_index = (flipped_y * orig_w + cur_orig_x) * 3;

            uint8_t b = pixels[pixel_index];
            uint8_t g = pixels[pixel_index + 1];
            uint8_t r = pixels[pixel_index + 2];

            uint32_t color = (r << 16) | (g << 8) | b;
            vga_put_pixel(start_x + x, start_y + y, color);
        }
    }
}
