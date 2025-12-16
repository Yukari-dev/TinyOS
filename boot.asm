[BITS 16]
[ORG 0x7C00]


start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    
    call clear_screen

    mov si, message_1
    call print_string
    xor al, al

    call wait_key

    jmp $

clear_screen:
    mov ah, 0x00
    mov al, 0x03
    int 0x10
    ret


wait_key:
    xor ah, ah
.loop:
    mov ah, 0x00
    int 0x16

    cmp al, 0
    je .special_key

    cmp al, 0x08
    je .backspace

    cmp al, 0x0D
    je .done

    cmp bx, 31
    jae .loop

    mov ah, 0x0E
    int 0x10
    
    mov [di], al
    inc di
    inc bx

    jmp .loop

.special_key:
    cmp ah, 0x48
    je .arrow_up
    cmp ah, 0x50
    je .arrow_down
    cmp ah, 0x4B
    je .arrow_left
    cmp ah, 0x4D
    je .arrow_right
    jmp .loop

.arrow_up:
    mov ah, 0x03
    xor bh, bh
    int 0x10

    dec dh
    mov ah, 0x02
    int 0x10

    jmp .loop

.arrow_down:
    mov ah, 0x03
    xor bh, bh
    int 0x10

    inc dh
    mov ah, 0x02
    int 0x10
    jmp .loop

.arrow_left:
    mov ah, 0x03
    xor bh, bh
    int 0x10

    dec dl
    mov ah, 0x02
    int 0x10
    jmp .loop

.arrow_right:
    mov ah, 0x03
    xor bh, bh
    int 0x10

    inc dl
    mov ah, 0x02
    int 0x10
    jmp .loop
.backspace:
    mov ah, 0x08
    xor bh, bh
    int 0x10

    cmp dl, 0
    je .loop
    
    dec dl
    mov ah, 0x02
    int 0x10

    mov ah, 0x0A
    mov al, ' '
    xor bh, bh
    mov cx, 1
    int 0x10
    jmp .loop

.done:
    mov byte [di], 0
    mov ah, 0x0E
    mov al, 0x0D
    int 0x10
    mov al, 0x0A
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
