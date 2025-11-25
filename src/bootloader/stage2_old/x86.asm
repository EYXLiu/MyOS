bits 16

section _TEXT class=CODE

;
; U4D
; unsigned 4 byte divide
;
global __U4D
__U4D:
    shl edx, 16
    mov dx, ax
    mov eax, edx
    xor edx, edx

    shl ecx, 16
    mov cx, bx

    div ecx
    mov ebx, edx
    mov ecx, edx
    shr ecx, 16
    mov edx, eax
    shr edx, 16

    ret


;
; U4M
; unsigned 4 byte multiply
;
global __U4M
__U4M:
    shl edx, 16
    mov dx, ax
    mov eax, edx

    shl ecx, 16
    mov cx, bx
    
    mul ecx
    mov edx, eax
    shr edx, 16

    ret

;
; void _cdecl x86_div64_32(uint64_t dividend, uint32_t divisor, uint64_t* quotientOut, uint32_t remainderOut);
; divides the dividend by the divisor
;
global _x86_div64_32
_x86_div64_32:

    ; make new call frame
    push bp
    mov bp, sp

    push bx

    ; divide upper bits
    mov eax, [bp + 8] ; upper 32 bits of dividend
    mov ecx, [bp + 12] ; divisor
    xor edx, edx
    div ecx
    ; store upper bits to output
    mov ebx, [bp + 16]
    mov [bx + 4], eax

    ; divide lower bits
    mov eax, [bp + 4] ; lower 32 bits of dividend
    div ecx
    ;store lower bits
    mov [bx], eax
    mov bx, [bp + 18]
    mov [bx], edx

    pop bx

    ; restore old call frame
    mov sp, bp
    pop bp
    ret


;
; void _cdecl x86_Video_WriteCharTeletype(char c, uint8_t page);
; prints a character
;
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

    ; restore old call frame

    mov sp, bp
    pop bp
    ret

;
; bool _cdecl x86_Disk_Reset(uint8_t drive);
; reset disk
;
global _x86_Disk_Reset
_x86_Disk_Reset:
    push bp
    mov bp, sp

    ; just call 0x13
    mov ah, 0
    mov dl, [bp + 4]
    stc
    int 0x13
    
    mov ax, 1
    sbb ax, 0

    mov sp, bp
    pop bp
    ret

;
; bool _cdecl x86_Disk_Read(uint8_t drive, uint16_t cylinder, uint16_t sector, uint16_t head, uint8_t count, uint8_t far* dataOut);
; reads from disk
;
global _x86_Disk_Read
_x86_Disk_Read:
    push bp
    mov bp, sp

    push bx
    push es

    mov dl, [bp + 4] ; drive number
    mov ch, [bp + 6] ; cylinder
    mov cl, [bp + 7]
    shl cl, 6
    mov al, [bp + 8] ; sector
    and al, 0x3F
    or cl, al
    mov dh, [bp + 10] ; head
    mov al, [bp + 12] ; count
    mov bx, [bp + 16] ; far pointer 
    mov es, bx
    mov bx, [bp + 14]

    ; call 0x13 with flag ah = 0x2
    mov ah, 0x2
    stc
    int 0x13
    
    mov ax, 1
    sbb ax, 0

    pop es
    pop bx

    mov sp, bp
    pop bp
    ret

;
; bool cdecl x86_Disk_GetDriveParams(uint8_t drive, uint8_t* driveTypeOut, uint16_t* cylindersOut, uint16_t* sectorsOut, uint16_t* headsOut);
; gets driver params
;
global _x86_Disk_GetDriveParams
_x86_Disk_GetDriveParams:
    push bp
    mov bp, sp

    push es
    push di
    push bx
    push si

    mov dl, [bp + 4] ; drive number
    mov ah, 0x8
    mov di, 0
    mov es, di
    stc
    int 0x13
    
    mov ax, 1
    sbb ax, 0

    ; out params
    mov si, [bp + 6] ; drive type
    mov [si], bl

    mov bl, ch ; cylinders
    mov bh, cl
    shr bh, 6
    mov si, [bp + 8]
    mov [si], bx

    xor ch, ch ; sectors
    and cl, 0x3f
    mov si, [bp + 10]
    mov [si], cx

    mov cl, dh ; heads
    mov si, [bp + 12]
    mov [si], cx

    pop si
    pop bx
    pop di
    pop es

    mov sp, bp
    pop bp
    ret
