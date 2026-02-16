#ifndef _STDINT_H
#define _STDINT_H

// Fixed-width unsigned integers
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long long uint64_t;

// Fixed-width signed integers
typedef signed char        int8_t;
typedef signed short       int16_t;
typedef signed int         int32_t;
typedef signed long long   int64_t;

// Standard sizes and pointers
typedef uint32_t           size_t;
typedef uint32_t           uintptr_t;

#define NULL ((void*)0)

#endif
