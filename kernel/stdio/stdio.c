#include "stdio.h"
#include <stdarg.h>
#include "../vga/vga.h"

ColorMap colors[] = {
    {'k', 0x00}, // Black
    {'b', 0x01}, // Blue (Dark)
    {'g', 0x02}, // Green (Dark)
    {'c', 0x03}, // Cyan (Dark)
    {'r', 0x04}, // Red (Dark)
    {'m', 0x05}, // Magenta (Dark)
    {'o', 0x06}, // Brown (Closest to Orange)
    {'7', 0x07}, // Light Gray (Default)
    {'8', 0x08}, // Dark Gray
    {'B', 0x09}, // Light Blue
    {'G', 0x0A}, // Light Green
    {'C', 0x0B}, // Light Cyan
    {'R', 0x0C}, // Light Red
    {'P', 0x0D}, // Light Magenta / Pink
    {'Y', 0x0E}, // Yellow
    {'W', 0x0F}  // White (Bright)
};

void clear(){
    for (int i = 0; i < width * height; i++){
        video_mem[i] = (unsigned short)' ' | (0x07 << 8);
        cursor_x = 0;
        cursor_y = 0;
        update_cursor();
    }
}

void printf(const char default_color, const char* format, ...){
    va_list args;
    va_start(args, format);

    for(int i = 0; format[i] != '\0'; i++){
        if(format[i] == '%'){
            i++;
            char type = format[i];
            unsigned char color = default_color;
            char potential_code = format[i + 1];
            unsigned char found = find_color(potential_code);
            if(found != 255){
                color = found;
                i++;
            }
            switch(type){
                case 's':
                    char* s = va_arg(args, char*);
                    while(*s) printc(*s++, color);
                    break;
                case 'c':
                    char c = (char)va_arg(args, int);
                    printc(c, color);
                    break;
                case 'i':
                    int num = va_arg(args, int);
                    char* n = num_to_char(num);
                    while(*n) printc(*n++, color);
                    break;
            }
        } else{
            printc(format[i], default_color);
        }
    }
    va_end(args);
    update_cursor();
}

char int_buffer[32];

char* num_to_char(int num){
    if(num == 0) return "0";

    int i = 30;
    int_buffer[31] = '\0';

    while(num > 0 && i > 0){
        int_buffer[i] = (num % 10) + '0';
        num /= 10;
        i--;
    }

    return &int_buffer[i + 1];
}

short len(char* c){
    short count = 0;
    while(*c != '\0'){
        count++;
        *c++;
    }
    return count;
}

void printc(char c, const char color){
    if(c == '\n'){
        cursor_y++;
        cursor_x = 0;
    } else{
        int index = cursor_y * width + cursor_x;
        video_mem[index] = (unsigned short)c | (unsigned short)color << 8;
        cursor_x++;
    }

    if(cursor_x >= width){
        cursor_x = 0;
        cursor_y++;
    }

    if(cursor_y >= height){
        scroll();
    }

    update_cursor();
}


unsigned char find_color(char c){
    for(int i = 0; i < 16; i++){
        if(colors[i].code == c) return colors[i].color_val;
    }
    return 255;
}


void scroll(){
    for(int y = 1; y < height; y++){
        for(int x = 0; x < width; x++){
            int dest_index = (y - 1) * width + x;
            int index = y * width + x;
            video_mem[dest_index] = video_mem[index];
        }
    }

    int last_line_start = (height - 1) * width;
    for (int x = 0; x < width; x++) {
        video_mem[last_line_start + x] = (unsigned short)' ' | (0x07 << 8);
    }

    cursor_y = height - 1;
}


