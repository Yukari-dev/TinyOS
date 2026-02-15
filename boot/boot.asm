; boot.asm - simple x86 bootloader
[BITS 16] ; real mode
[ORG 0x7C00] ; the origin of the bootloader in the memory

start:
    cli             
    xor ax, ax      
    mov ds, ax      
    mov es, ax      
    mov ss, ax      
    mov sp, 0x7C00  
    
    ; 1. Load kernel from disk
    mov ah, 0x02 
    mov al, 50    
    mov ch, 0     
    mov cl, 2     
    mov dh, 0     
    mov bx, 0x1000 
    int 0x13      
    jc disk_err 

    ; 2. SWITCH TO GRAPHICS MODE HERE (While still in Real Mode)
    mov ax, 0x0013
    int 0x10

    ; 3. Transition to Protected Mode
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:protected_mode

print_string:
    lodsb
    or al, al
    je .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

disk_err:
    mov si, dsk_err
    call print_string
    ret

gdt_start:
    dq 0 
gdt_code:
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0
gdt_data:
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
protected_mode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    jmp 0x1000

wlcm_msg db "Booting To T-OS...", 13, 10, 0
dsk_err db "Failed to load the disk.", 13, 10, 0



times 510-($-$$) db 0
dw 0xaa55
