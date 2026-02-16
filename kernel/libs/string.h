#include "stdint.h"
#ifndef STRING_H
#define STRING_H

void* memset(void* dest, int val, size_t n);

void* memcpy(void* dest, const void* src, size_t n);

size_t strlen(const char* s);

char* itoa(int value, char* str, int base);

#endif
