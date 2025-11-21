org 0x0
bits 16

%define ENDL 0x0D, 0x0A

_start:

    ; code 
    mov si, hello
    call prints
.halt:
    cli
    hlt

prints:
    push si
    push ax
.loop:
    lodsb
    or al, al
    jz .done

    mov ah, 0x0e
    mov bh, 0
    int 0x10

    jmp .loop
.done:
    pop ax
    pop si
    ret

hello:
    db 'Hello world!', ENDL, 0
