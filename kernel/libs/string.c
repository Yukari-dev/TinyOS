#include "string.h"

void* memcpy(void* dest, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dest;
    const uint8_t* s = (const uint8_t*)src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void* memset(void* s, int c, size_t n) {
    uint8_t* p = (uint8_t*)s;
    while (n--) {
        *p++ = (uint8_t)c;
    }
    return s;
}

int memcmp(const void* s1, const void* s2, size_t n){
    const uint8_t* p1 = (const uint8_t*)s1;
    const uint8_t* p2 = (const uint8_t*)s2;

    for(size_t i = 0; i < n; i++){
        if(p1[i] != p2[i]){
            return (p1[i] - p2[i]);
        }
    }
    return 0;
}

size_t strlen(const char* s) {
    size_t len = 0;
    while (s[len]) len++;
    return len;
}

char* itoa(int value, char* str, int base){
    char *rc, *ptr, *low;
    int tmp_value;
    if(base < 2 || base > 36) return str;

    rc = ptr = str;

    if(value < 0 && base == 10) *ptr = '-';

    low = ptr;
    
    do{
        tmp_value = value;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[value % base];
        value /= base;
    } while(value);

    // Apply negative sign for base 10
    if (tmp_value < 0 && base == 10) *ptr++ = '-';

    *ptr-- = '\0';

    while(low < ptr){
        char tmp = *low;
        *low++ = *ptr;
        *ptr-- = tmp;
    }
    return rc;
}
