#include "idt.h"
#include "../vga/vga.h"

idt_entry_t idt[256];
idt_ptr_t idt_reg;

void set_idt_gate(int n, unsigned int handler){
    idt[n].base_low = (unsigned int)(handler & 0xFFFF);
    idt[n].sel = 0x08; // Kernel code segment (defined in your GDT)
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; // 0x8E = Present, Ring 0, 32-bit Interrupt Gate
    idt[n].base_high = (unsigned int)((handler >> 16) & 0xFFFF);
}

void load_idt(){
    idt_reg.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_reg.base = (unsigned int)&idt;
    // Tell the CPU where the IDT is using Assembly
    __asm__ __volatile__("lidt (%0)" : : "r" (&idt_reg));
}

void pic_remap(){
    // ICW1 - initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    // ICW2 - remap offset address
    outb(0x21, 0x20); // Master PIC jumps to 0x20 (32)
    outb(0xA1, 0x28); // Slave PIC jumps to 0x28 (40)
    // ICW3 - cascading
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    // ICW4 - environment info
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    // Mask interrupts (0x00 enables all, 0xFD enables keyboard only)
    outb(0x21, 0xFD); 
    outb(0xA1, 0xFF);
}

extern void keyboard_wrapper(); // Declare the assembly label

void init_keyboard_idt() {
    // 33 is the IDT index for Keyboard (IRQ 1 + 32 offset)
    set_idt_gate(33, (unsigned int)keyboard_wrapper);
}

