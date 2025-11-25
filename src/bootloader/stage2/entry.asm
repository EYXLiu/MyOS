bits 16

section .entry

extern __bss_start
extern __end

extern cstart
global entry

entry:
    ; set up stack
    cli

    ; save boot drive
    mov [g_BootDrive], dl

    mov ax, ds
    mov ss, ax
    mov sp, 0xFFF0
    mov bp, sp

    ; switch to protected mode
    call EnableA20
    call LoadGDT

    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp dword 0x8:.pmode

; now in protected mode
.pmode:
    [bits 32]
    
    ; setup stack
    mov ax, 0x10
    mov ds, ax
    mov ss, ax

    ; empty bss (uninitialized data)
    mov edi, __bss_start
    mov ecx, __end
    sub ecx, edi
    mov al, 0
    cld
    rep stosb

    ; run c code
    xor edx, edx
    mov dl, [g_BootDrive]
    push edx
    call cstart

    cli
    hlt

; for backwards compatibility
; when trying to access A20, it would wrap back to beginning
EnableA20:
    [bits 16]
    ; disable keyboard
    call A20WaitInput
    mov al, KbdControllerDisableKeyboard
    out KbdControllerCommandPort, al

    ; read control output port
    call A20WaitInput
    mov al, KbdControllerReadCtrlOutputPort
    out KbdControllerCommandPort, al

    call A20WaitOutput
    in al, KbdControllerDataPort
    push eax

    ; write control output port
    call A20WaitInput
    mov al, KbdControllerWriteCtrlOutputPort
    out KbdControllerCommandPort, al
    
    call A20WaitInput
    pop eax
    or al, 2 
    out KbdControllerDataPort, al

    ; enable keyboard
    call A20WaitInput
    mov al, KbdControllerEnableKeyboard
    out KbdControllerCommandPort, al

    call A20WaitInput
    ret


A20WaitInput:
    [bits 16]
    ; wait until status bit 2 (input buffer) is 0
    in al, KbdControllerCommandPort
    test al, 2
    jnz A20WaitInput
    ret

A20WaitOutput:
    [bits 16]
    ; wait until status bit 1 (output buffer) is 1 so it can be read
    in al, KbdControllerCommandPort
    test al, 1
    jz A20WaitOutput
    ret


LoadGDT:
    [bits 16]
    lgdt [g_GDTDesc]
    ret



KbdControllerDataPort equ 0x60
KbdControllerCommandPort equ 0x64
KbdControllerDisableKeyboard equ 0xAD
KbdControllerEnableKeyboard equ 0xAE
KbdControllerReadCtrlOutputPort equ 0xD0
KbdControllerWriteCtrlOutputPort equ 0xD1

ScreenBuffer equ 0xB8000

; global discriptor table
; for segmenting sections into different access levels
g_GDT:
    dq 0

    ; flat mapping (no segmentation)
    ; 32-bit code segment
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

    ; 32-bit data segment
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0

    ; for switch back to real mode
    ; 16-bit code segment
    dw 0xFFFF
    dw 0
    db 0
    db 10011010b
    db 00001111b
    db 0

    ; 16-bit data segment
    dw 0xFFFF
    dw 0
    db 0
    db 10010010b
    db 00001111b
    db 0

g_GDTDesc:  
    ; limit = size of GDT
    dw g_GDTDesc - g_GDT - 1
    ; address of GDT 
    dd g_GDT

g_BootDrive:
    db 0
