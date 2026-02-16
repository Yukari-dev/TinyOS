[BITS 32]
[global _start]
[global keyboard_wrapper]   ; Make the wrapper visible to idt.c
[extern main]
[extern keyboard_handler] ; The function in your keyboard.c

_start:
    call main
    jmp $

; This is the bridge between the Hardware Interrupt and your C code
keyboard_wrapper:
    pusha
    ; Don't change DS/ES yet, let's see if it works with current segments
    call keyboard_handler
    popa
    iretd
