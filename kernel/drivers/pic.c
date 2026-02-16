#include "pic.h"

static inline void io_wait(void) {
    outb(0x80, 0);
}

void pic_init(void) {
    // Save masks
    unsigned short mask1 = inb(PIC1_DATA);
    unsigned short mask2 = inb(PIC2_DATA);
    
    // Start initialization sequence
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    
    // Set vector offsets
    outb(PIC1_DATA, 0x20); // Master PIC offset to 0x20 (IRQ0-7 = INT 0x20-0x27)
    io_wait();
    outb(PIC2_DATA, 0x28); // Slave PIC offset to 0x28 (IRQ8-15 = INT 0x28-0x2F)
    io_wait();
    
    // Tell Master PIC there's a slave at IRQ2
    outb(PIC1_DATA, 0x04);
    io_wait();
    
    // Tell Slave PIC its cascade identity
    outb(PIC2_DATA, 0x02);
    io_wait();
    
    // Set to 8086 mode
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();
    
    // Restore saved masks
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_enable_irq(unsigned short irq) {
    unsigned int port;
    unsigned short value;
    
    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}

void pic_disable_irq(unsigned short irq) {
    unsigned int port;
    unsigned short value;
    
    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    
    value = inb(port) | (1 << irq);
    outb(port, value);
}
