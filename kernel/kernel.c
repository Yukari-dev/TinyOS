#include "fs/fs.h"
#include "vga/vga.h"
#include "stdio/stdio.h"
#include "cpu/idt.h"
#include "shell/shell.h"
#include "memory/heap.h"


void initializing(){
    disable_blinking();

    printf(0x07, "[%so]Initializing T-OS...\n", ".");

    printf(0x07, "[%so]Keyboard IDT is initializing...\n", ".");
    init_keyboard_idt();
    printf(0x07, "[%sg]Keyboard IDT is inialized.\n", "+");

    printf(0x07, "[%so]Remaping the PIC in process...\n", ".");
    pic_remap();
    printf(0x07, "[%sg]PIC is successfully remaped.\n", "+");

    printf(0x07, "[%so]Loading IDT to CPU in process...\n", ".");
    load_idt();
    printf(0x07, "[%sg]IDT is successfully loaded to CPU.\n", "+");

    printf(0x07, "[%so]Enabeling the interrupts...\n", ".");
    __asm__ __volatile__("sti");
    printf(0x07, "[%sg]interrupts are enabled.\n", "+");

    init_ramdisk();
    init_heap();

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

