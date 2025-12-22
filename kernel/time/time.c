#include "time.h"
#include "../stdio/stdio.h"
#include "../vga/vga.h"

volatile unsigned int timer_ticks = 0;
unsigned char second = 0;
unsigned char minute = 0;
unsigned char hour   = 0;

void init_timer(unsigned int frequency){
    unsigned short devisor = 1193180 / frequency;

    outb(0x43, 0x36);

    unsigned char l = (unsigned char)(devisor & 0xFF);
    unsigned char h = (unsigned char)((devisor << 8) && 0xFF);

    outb(0x40, l);
    outb(0x40, h);
}

unsigned char get_update_in_progress_flag(){
    outb(0x70, 0x0A);
    return (inb(0x71) & 0x80);
}

unsigned char get_rtc_register(int reg){
    outb(0x70, reg);
    return inb(0x71);
}

unsigned char last_second;

void update_clock_display() {
    while(get_update_in_progress_flag());
    unsigned char second = get_rtc_register(0x00);
    unsigned char minute = get_rtc_register(0x02);
    unsigned char hour   = get_rtc_register(0x04);

    second = (second & 0x0F) + ((second / 16) * 10);
    minute = (minute & 0x0F) + ((minute / 16) * 10);
    hour = (hour+1 & 0x0F) + ((hour / 16) * 10);

    if(last_second != second){
        timer_ticks++;
        last_second = second;
    }

    char clock_str[11];
    clock_str[0] = '[';
    clock_str[1] = (hour / 10) + '0';
    clock_str[2] = (hour % 10) + '0';
    clock_str[3] = ':';
    clock_str[4] = (minute / 10) + '0';
    clock_str[5] = (minute % 10) + '0';
    clock_str[6] = ':';
    clock_str[7] = (second / 10) + '0';
    clock_str[8] = (second % 10) + '0';
    clock_str[9] = ']';
    clock_str[10] = '\0';

    print_at(69, 0, clock_str, 0xF0); // 0x0E is Yellow on Black
}

void sleep(unsigned int ticks){
    unsigned int start_time = timer_ticks;
    while(timer_ticks < start_time + ticks){
        update_clock_display();
    }
}

