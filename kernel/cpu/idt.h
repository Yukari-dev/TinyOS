#ifndef IDT_H
#define IDT_H

typedef struct{
    unsigned short base_low;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_high;
}__attribute__((packed)) idt_entry_t;

typedef struct{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed)) idt_ptr_t;

extern idt_entry_t idt[256];
extern idt_ptr_t idt_reg;

void set_idt_gate(int n, unsigned int handler);

void load_idt();

void pic_remap();

void init_keyboard_idt();

#endif
