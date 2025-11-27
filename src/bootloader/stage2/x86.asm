%macro x86_EnterRealMode 0
    [bits 32]
    jmp word 0x18:.pmode16
.pmode16:
    [bits 16]
    ; disable protected mode bit
    mov eax, cr0
    and al, ~1
    mov cr0, eax
    ; jump to real mode
    jmp word 0x00:.rmode
.rmode:
    mov ax, 0
    mov ds, ax
    mov ss, ax
    sti
%endmacro

%macro x86_EnterProtectedMode 0
    [bits 16]
    cli
    ; set protected bit
    mov eax, cr0
    or al, 1
    mov cr0, eax
    ; jump to protected mode
    jmp dword 0x08:.pmode
.pmode:
    [bits 32]
    ; setup segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
%endmacro
;
; convert linear address to segment:offset address
; linear address
; target segment
; target 32 bit register high
; target 32 bit register low
;
%macro LinearToSegOffset 4
    mov %3, %1 ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1
    and %3, 0xF
%endmacro

global x86_outb
x86_outb:
    [bits 32]
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

global x86_inb
x86_inb:
    [bits 32]
    mov dx, [esp + 4]
    xor eax, eax
    in al, dx
    ret

;
; bool _cdecl x86_Disk_Reset(uint8_t drive);
; reset disk
;
global x86_Disk_Reset
x86_Disk_Reset:
    [bits 32]

    push ebp
    mov ebp, esp 


    x86_EnterRealMode

    ; just call 0x13
    mov ah, 0
    mov dl, [bp + 8] 
    stc
    int 0x13

    mov eax, 1
    sbb eax, 0  

    push eax

    x86_EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

;
; bool _cdecl x86_Disk_Read(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t count, uint8_t far* dataOut);
; reads from disk
;
global x86_Disk_Read
x86_Disk_Read:
    [bits 32]

    push ebp
    mov ebp, esp

    x86_EnterRealMode

    push ebx
    push es

    ; drive number
    mov dl, [bp + 8]

    ; cylinder
    mov ch, [bp + 12]
    mov cl, [bp + 13]
    shl cl, 6
    
    ; sector
    mov al, [bp + 16]
    and al, 0x3F
    or cl, al
    ; head 
    mov dh, [bp + 20]

    ; count
    mov al, [bp + 24] 

    ; pointer 
    LinearToSegOffset [bp + 28], es, ebx, bx

    ; call 0x13 with flag ah = 0x2
    mov ah, 0x02
    stc
    int 0x13

    mov eax, 1
    sbb eax, 0 

    pop es
    pop ebx

    push eax

    x86_EnterProtectedMode

    pop eax

    ; restore old call frame
    mov esp, ebp
    pop ebp
    ret

;
; bool cdecl x86_Disk_GetDriveParams(uint8_t drive, uint8_t* driveTypeOut, uint16_t* cylindersOut, uint16_t* sectorsOut, uint16_t* headsOut);
; gets driver params
;
global x86_Disk_GetDriveParams
x86_Disk_GetDriveParams:
    [bits 32]

    push ebp 
    mov ebp, esp 

    x86_EnterRealMode

    [bits 16]

    push es
    push bx
    push esi
    push di

    mov dl, [bp + 8] 
    mov ah, 0x08
    mov di, 0
    mov es, di
    stc
    int 0x13

    mov eax, 1
    sbb eax, 0

    ; drive type
    LinearToSegOffset [bp + 12], es, esi, si
    mov [es:si], bl

    ; cylinders
    mov bl, ch 
    mov bh, cl 
    shr bh, 6
    inc bx

    LinearToSegOffset [bp + 16], es, esi, si
    mov [es:si], bx

    ; sectors
    xor ch, ch
    and cl, 0x3F
    
    LinearToSegOffset [bp + 20], es, esi, si
    mov [es:si], cx

    ; heads
    mov cl, dh 
    inc cx

    LinearToSegOffset [bp + 24], es, esi, si
    mov [es:si], cx

    pop di
    pop esi
    pop bx
    pop es

    push eax

    x86_EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

global x86_Video_GetVbeInfo
x86_Video_GetVbeInfo:
    [bits 32]
    push ebp 
    mov ebp, esp 
    x86_EnterRealMode

    push edi
    push es

    mov eax, 0x4F00
    LinearToSegOffset [bp + 8], es, edi, di
    int 0x10

    pop es
    pop edi

    push eax
    x86_EnterProtectedMode
    pop eax

    mov esp, ebp
    pop ebp
    ret

global x86_Video_GetModeInfo
x86_Video_GetModeInfo:
    [bits 32]
    push ebp 
    mov ebp, esp 
    x86_EnterRealMode

    push edi
    push es
    push ecx

    mov eax, 0x4F01
    mov cx, [bp + 8]
    LinearToSegOffset [bp + 12], es, edi, di
    int 0x10

    pop ecx
    pop es
    pop edi

    push eax
    x86_EnterProtectedMode
    pop eax

    mov esp, ebp
    pop ebp
    ret

global x86_Video_SetMode
x86_Video_SetMode:
    [bits 32]
    push ebp 
    mov ebp, esp 
    x86_EnterRealMode

    push edi
    push es
    push ebx

    mov ax, 0
    mov es, ax
    mov edi, 0
    mov eax, 0x4F02
    mov bx, [bp + 8]
    int 0x10

    pop ebx
    pop es
    pop edi

    push eax
    x86_EnterProtectedMode
    pop eax

    mov esp, ebp
    pop ebp
    ret
