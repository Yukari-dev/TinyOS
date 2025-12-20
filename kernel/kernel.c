#include "vga/vga.h"
#include "stdio/stdio.h"
#include "cpu/idt.h"
#include "shell/shell.h"

void initializing(){
    printf(0x07, "[%sg]Initializing T-OS...\n", "+");

    printf(0x07, "[%sg]Keyboard IDT is initializing...\n", "+");
    init_keyboard_idt();
    printf(0x07, "[%sg]gKeyboard IDT is inialized.\n", "+");

    printf(0x07, "[%sg]Remaping the PIC in process...\n", "+");
    pic_remap();
    printf(0x07, "[%sg]PIC is successfully remaped.\n", "+");

    printf(0x07, "[%sg]Loading IDT to CPU in process...\n", "+");
    load_idt();
    printf(0x07, "[%sg]IDT is successfully loaded to CPU.\n", "+");

    printf(0x07, "[%sg]Enabeling the interrupts...\n", "+");
    __asm__ __volatile__("sti");
    printf(0x07, "[%sg]interrupts are enabled.\n", "+");

    printf(0x07, "Welcome to T-OS!\n");

    enable_cursor(14, 15);
    update_cursor();

}

void main(){
    clear();

    initializing();
    
    // FOR SHELL
    initialize();

    while(1);
}

