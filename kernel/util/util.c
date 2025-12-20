#include "util.h"

void outb(unsigned short port, unsigned char data){
    __asm__("outb %0, %1" : : "a"(data), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outw(unsigned short port, unsigned short data) {
    __asm__ __volatile__("outw %0, %1" : : "a"(data), "Nd"(port));
}
