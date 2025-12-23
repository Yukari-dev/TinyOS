#ifndef VGA_H
#define VGA_H

extern unsigned short* video_mem;
extern unsigned short width;
extern unsigned short height;

extern unsigned short cursor_y;
extern unsigned short cursor_x;

void init_graphics_mode();

void disable_blinking();

void update_cursor();

void enable_cursor(unsigned char cursor_start, unsigned char cursor_end);


#endif
