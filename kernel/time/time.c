#include "time.h"
#include "../util/util.h" // for outb/inb

volatile uint32_t system_ticks = 0;

void timer_init(uint32_t freq) {
    // The PIT has an internal clock of 1.193182 MHz
    uint32_t divisor = 1193180 / freq;

    // Send the command byte (0x36): 
    // Binary counter, Mode 3 (Square wave), Load LSB then MSB, Channel 0
    outb(0x43, 0x36);

    // Divisor must be sent in two bytes (Low then High)
    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40, l);
    outb(0x40, h);
}

// This function is called by your IDT (Interrupt Descriptor Table)
// usually on IRQ0
void timer_handler() {
    system_ticks++;
}
