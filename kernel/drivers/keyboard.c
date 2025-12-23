#include "keyboard.h"
#include "../stdio/stdio.h" 
#include "../util/util.h" 
#include "../vga/vga.h" 
#include "../shell/shell.h" 

unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
    '9', '0', '-', '=', '\b', /* Backspace */
    '\t', /* Tab */
    'q', 'w', 'e', 'r', /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0, /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
    '\'', '`', 0, /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 49 */
    'm', ',', '.', '/', 0, /* Right shift */
    '*',
    0, /* Alt */
    ' ', /* Space bar */
    0, /* Caps lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* F1-F10 */
    0, /* 69 - Num lock*/
    0, /* Scroll Lock */
    '7', '8', '9', '-',
    '4', '5', '6', '+',
    '1', '2', '3', '0', '.', 0, 0, 0, 0, 0, /* F11, F12... */
};

unsigned char shift_map[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', 
    '(', ')', '_', '+', '\b', 
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*',
    0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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

unsigned short shift_pressed = 0;

char cmd_history[5][64];
int history_count = 0;
short history_view_index = -1;
short max_history_count = 5;

void keyboard_handler() {
    unsigned char scancode = inb(0x60);
    outb(0x20, 0x20);

    if (scancode & 0x80) {
        unsigned char released = scancode & 0x7F;
        if (released == 0x2A || released == 0x36) {
            shift_pressed = 0;
        }
        return;    }

    if (scancode == 0x2A || scancode == 0x36) {
        shift_pressed = 1;
        return; 
    }

    // 2. Check for Enter Key
    if (scancode == 0x1C) {
        if(input_index > 0){
            input_buffer[input_index] = '\0';

            for(int i = max_history_count - 1; i > 0; i--){
                strcpy(cmd_history[i - 1], cmd_history[i]);
            }
            strcpy(input_buffer, cmd_history[0]);
            
            if(history_count < max_history_count) history_count++;

            printf(0x07, "\n");

            execute_cmd(input_buffer);

            history_view_index = -1;

            clear_input(input_buffer);
            update_cursor();
            return;
        }
        initialize(); 
    }

    // 3. Check for Backspace
    if (scancode == 0x0E) {
        if (input_index > 0 && is_shell == 'y') {
            input_index--;
            input_buffer[input_index] = '\0';
            
            if (cursor_x > 1) {
                cursor_x--;
                int index = cursor_y * width + cursor_x;
                video_mem[index] = (unsigned short)' ' | (unsigned short)0x07 << 8;
            }
        }
        update_cursor();
        return;
    }
    
    // Up Arrow
    if(scancode == 0x48){
        strcpy('\0', input_buffer);
        if(history_view_index < history_count - 1){
            history_view_index++;

            while(input_index > 0){
                input_index--;
                input_buffer[input_index] = '\0';
                
                if (cursor_x > 1) {
                    cursor_x--;
                    int index = cursor_y * width + cursor_x;
                    video_mem[index] = (unsigned short)' ' | (unsigned short)0x07 << 8;
                }

            }
            strcpy(cmd_history[history_view_index], input_buffer);
            input_index = len(input_buffer);

            printf(0x0F, "%s", input_buffer);
        }
        return;
    }
    
    // Down Arrow
    if(scancode == 0x50){
        return;
    }
    
    // Left Arrow
    if(scancode == 0x4B){
        if(cursor_x > 1){
            cursor_x--;
        }
        update_cursor();
        return;
    }
    
    // Right Arrow
    if(scancode == 0x4D){
        if(cursor_x < input_index){
            cursor_x++;
        }
        update_cursor();
        return;
    }

    // 4. Handle Character Keys
    char letter = shift_pressed ? shift_map[scancode] : keyboard_map[scancode];

    if (letter > 0) {
        if (input_index < 127) {
            printc(letter, 0x07);
            input_buffer[input_index] = letter;
            input_index++;
            update_cursor();
        }
    }
}
