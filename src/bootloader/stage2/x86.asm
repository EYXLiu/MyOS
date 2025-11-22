bits 16

section _TEXT class=CODE

global _x86_Video_WriteCharTeletype
_x86_Video_WriteCharTeletype:

    ; make new call frame
    push bp
    mov bp, sp

    push bx

    ; [bp + 0] old call frame
    ; [bp + 2] return address
    ; [bp + 4] first argument
    ; [bp + 6] second argument
    mov ah, 0xE
    mov al, [bp + 4]
    mov bh, [bp + 6]
    int 0x10

    pop bx

    mov sp, bp
    pop bp
    ret
