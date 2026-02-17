#ifndef TIME_H
#define TIME_H

#include "../libs/stdint.h"

extern volatile uint32_t system_ticks;

void timer_init(uint32_t freq);
void timer_handler();

#endif
