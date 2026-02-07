#ifndef KEYBOARD_H
#define KEYBOARD_H

extern unsigned char keyboard_map[128];

extern unsigned short buffer_pos;

void clear_input(unsigned char *input);
void show_input(unsigned char input[128]);
void keyboard_handler();

#endif
