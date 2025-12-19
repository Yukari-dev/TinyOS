#ifndef VGA_H
#define VGA_H

static unsigned short* video_mem = (unsigned short*)0xB8000;
static unsigned short width = 80;
static unsigned short height = 25;

static unsigned short cursor_y = 0;
static unsigned short cursor_x = 0;

static void outb(unsigned short port, unsigned char data);

static void update_cursor();

#endif
