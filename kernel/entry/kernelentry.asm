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
    pusha               ; Save EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI
    
    push ds             ; Save Data Segments
    push es
    push fs
    push gs

    mov ax, 0x10        ; Load Kernel Data Segment
    mov ds, ax
    mov es, ax

    call keyboard_handler ; Run your C code

    pop gs              ; Restore everything
    pop fs
    pop es
    pop ds
    popa
    
    iretd               ; Special Interrupt Return
