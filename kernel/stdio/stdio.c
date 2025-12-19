#include "stdio.h"
#include <stdarg.h>
#include "../vga/vga.h"

void clear(){
    for (int i = 0; i < width * height; i++){
        video_mem[i] = 0x70;
    }
}

void printf(const char color, const char* format, ...){
    va_list args;
    va_start(args, format);

    for(int i = 0; format[i] != '\0'; i++){
        if(format[i] == '%'){
            i++;
            if(format[i] == 's'){
                char* s = va_arg(args, char*);
                while(*s) printc(*s++, 0x0F);
            } else if(format[i] == 'c'){
                printc((char)va_arg(args, int), 0x0F);
            }
        } else{
            printc(format[i], 0x0F);
        }
    }
    va_end(args);
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
}

