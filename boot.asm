[BITS 16]
[ORG 0x7C00]


start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    call clear_screen

    mov si, message_1
    call print_string

    jmp $

clear_screen:
    mov ah, 0x00
    mov al, 0x03
    int 0x10
    ret

print_string:
    mov ah, 0x0E
.loop:
    lodsb
    cmp al, 0x00
    je .done
    int 0x10
    jmp .loop
.done:
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
    int 0x10
    ret

message_1 db "Welcome to my OS!", 0
goodbye db "System halted.", 0
no_command db "unknown command.", 0
clear_cmd db "clear"
shutdown_cmd db "shutdown"

input_buffer times 32 db 0

times 510-($-$$) db 0
dw 0xAA55
