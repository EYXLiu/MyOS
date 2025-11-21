org 0x7C00
bits 16

%define ENDL 0x0D, 0x0A

;
; FAT12 header
;
jmp short _start
nop

bdb_oem: db "FROGGO  " ; 8 bytes
bdb_bytes_ps: dw 512
bdb_sectors_pc: db 1
bdb_reserved: dw 1
bdb_fat_count: db 2
bdb_dir_entry: dw 0xE0
bdb_sectors: dw 2880
bdb_media_type: db 0xF0
bdb_sectors_pf: dw 9
bdb_sectors_pt: dw 18
bdb_heads: dw 2
bdb_hidden_sectors: dd 0
bdb_large_sectors: dd 0

; boot extended
ebr_driver_number: db 0 ; 0x00 floppy
                   db 0
ebr_signature: db 0x29 ;signature
ebr_volume: db 0x11, 0x11, 0x11, 0x11 ; serial number
ebr_volume_label: db 'FROGGO     ' ; 11 bytes
ebr_system_id: db 'FAT12   ' ; 8 bytes



_start:
    jmp main

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

main:
    ; setup data segments 
    mov ax, 0
    mov ds, ax
    mov es, ax

    ; setup stack segments 
    mov ss, ax
    mov sp, 0x7C00

    ; code 
    mov [ebr_driver_number], dl
    mov ax, 1
    mov cl, 1
    mov bx, 0x7E00
    call disk_read

    mov si, msg_hello
    call prints

    cli
    hlt
;
; error handler
;
floppy_error:
    mov si, msg_read_failed
    call prints
    jmp .wait_key_and_reboot
    hlt

.wait_key_and_reboot:
    mov ah, 0
    int 0x16
    jmp 0xFFFF:0

.halt:
    cli
    jmp .halt

;
; disk routines
;

;
; LBA to CHS
; parameters:
;   ax : LBA
; return:
;   cx: [bits 0-5]: sector numbers
;   cx: [bits 6-15]: cylinder
;   dh: head
; 
lba_to_chs:
    push ax
    push dx

    xor dx, dx
    div word [bdb_sectors_pt] ; ax = LBA / sectors/track
                              ; dx = LBA % sectors/track

    inc dx
    mov cx, dx ;output to cx

    xor dx, dx
    div word [bdb_heads] ; ax = LBA / sectors/track / heads
                         ; dx = LBA / sectors/track % heads

    mov dh, dl ; dh = head
    mov ch, al ; ch = cylinder
    shl ah, 6 ; cl = cylinder (high bits)
    or cl, ah

    pop ax
    mov dl, al
    pop ax
    ret

;
; read from disk
; parameters
;   ax: LBA 
;   cl: sectors to read
;   dl: drive number
;   es:bx: memory address to store read
; 
disk_read:
    push ax
    push bx
    push cx
    push dx
    push di

    push cx
    call lba_to_chs
    pop ax
    
    mov ah, 0x2
    mov di, 3

.loop:
    pusha
    stc
    int 0x13
    jnc .done

    popa
    call disk_reset

    dec di
    test di, di
    jnz .loop

.fail:
    jmp floppy_error

.done
    popa

    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret

;
; disk reset
;
disk_reset:
    pusha
    mov ah, 0
    int 0x13
    jc floppy_error
    popa
    ret


msg_hello:
    db 'Hello world!', ENDL, 0
msg_read_failed:
    db 'Read from disk failed', ENDL, 0

times 510-($-$$) db 0
dw 0xAA55