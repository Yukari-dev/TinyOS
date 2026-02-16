#include "../util/util.h"
#ifndef PIC_H
#define PIC_H


// PIC (Programmable Interrupt Controller) ports
#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

// PIC commands
#define PIC_EOI 0x20

// Initialization Command Words
#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

void pic_init(void);
void pic_enable_irq(unsigned short irq);
void pic_disable_irq(unsigned short irq);

#endif
