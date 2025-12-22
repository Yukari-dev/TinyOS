#ifndef TIME_H
#define TIME_H

extern unsigned char second;
extern unsigned char minute;
extern unsigned char hour;

void init_timer(unsigned int frequency);

unsigned char get_update_in_progress_flag();

unsigned char get_rtc_register(int reg);

void update_clock_display();

void timer_callback();

void sleep(unsigned int ticks);

#endif
