#include "keyboard.h"
#include "../stdio/stdio.h" 
#include "../vga/vga.h" 
#include "../shell/shell.h" 

unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

unsigned char input_buffer[128];
unsigned short input_index = 0;

unsigned char is_shell = 'y';

void clear_input(unsigned char *input){
    for(int i = 0; i < 128; i++){
        input_buffer[i] = '\0';
    }
    input_index = 0;
}

void show_input(unsigned char *input){
    for(int i = 0; i < input_index; i++){
        printc(input[i], 0x0F);
    }
}

void keyboard_handler() {
    unsigned char scancode = inb(0x60);

    if(scancode & 0x80) {
        outb(0x20, 0x20);
        return;
    }
    if(scancode == 0x1C){
        input_buffer[input_index] = '\0';
        // next line (output)
        printf(0x07, "\n");

        if(input_index > 0){
            execute_cmd(input_buffer);
        } else{
            initialize();
        }

        //reset the input_buffer
        clear_input(input_buffer);
    } else if(scancode == 0x0E){
        if(cursor_x > 1 && is_shell == 'y'){
            cursor_x--;
            if(input_index > 0) input_index--;
            int index = cursor_y * width + cursor_x;
            video_mem[index] = (unsigned short)' ' | (unsigned short)0x07 << 8;
        }
    } else{
        char letter = keyboard_map[scancode];
        if(letter > 0){
            printc(letter, 0x07);
            if(input_index < 127){
                input_buffer[input_index] = letter;
                input_index++;
            }
        }
    }
    
    update_cursor();

    outb(0x20, 0x20);
}
