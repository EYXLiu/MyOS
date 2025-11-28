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
    ; setup data segments 
    mov ax, 0
    mov ds, ax
    mov es, ax

    ; setup stack segments 
    mov ss, ax
    mov sp, 0x7C00

    ; make sure BIOS starts in the proper location
    push es
    push word .after
    retf
.after:

    ; code 
    mov [ebr_driver_number], dl

    ; loading message
    mov si, msg_loading
    call prints

    ; check extensions present
    mov ah, 0x41
    mov bx, 0x55AA
    stc
    int 0x13

    jc .no_disk_extensions
    cmp bx, 0xAA55
    jne .no_disk_extensions
    
    ; extensions are present
    mov byte [have_extensions], 1
    jmp .after_disk_extention_check


.no_disk_extensions:
    mov byte [have_extensions], 0

.after_disk_extention_check:
    ; load stage 2
    mov si, stage2_location

    mov ax, STAGE2_LOAD_SEGMENT
    mov es, ax
    mov bx, STAGE2_LOAD_OFFSET

.loop:
    mov eax, [si]
    add si, 4
    mov cl, [si]
    inc si

    cmp eax, 0
    je .read_finish

    call disk_read

    xor ch, ch
    shl cx, 5
    mov di, es
    add di, cx
    mov es, di

    jmp .loop

.read_finish:
    ; far jump into stage2
    mov dl, [ebr_driver_number]

    mov ax, STAGE2_LOAD_SEGMENT
    mov ds, ax
    mov es, ax

    jmp STAGE2_LOAD_SEGMENT:STAGE2_LOAD_OFFSET

    ; should never happen
    jmp wait_key_and_reboot

    cli
    hlt
;
; error handler
;
floppy_error:
    mov si, msg_read_failed
    call prints
    jmp wait_key_and_reboot
    hlt

stage2_not_found_error:
    mov si, msg_stage2_not_found
    call prints
    jmp wait_key_and_reboot
    hlt

wait_key_and_reboot:
    mov ah, 0
    int 0x16
    jmp 0xFFFF:0

;
; print
;
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
;   eax: LBA 
;   cl: sectors to read
;   dl: drive number
;   es:bx: memory address to store read
; 
disk_read:
    push eax
    push bx
    push cx
    push dx
    push di

    ; check extensions
    cmp byte [have_extensions], 1
    jne .no_disk_extensions

    ; with extensions
    mov [extensions_dap.count], cl
    mov [extensions_dap.segment], es
    mov [extensions_dap.offset], bx
    mov [extensions_dap.lba], eax

    mov ah, 0x42
    mov si, extensions_dap
    mov di, 3
    int 0x13
    jmp .loop

.no_disk_extensions:
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

.done:
    popa

    pop di
    pop dx
    pop cx
    pop bx
    pop eax
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


msg_loading: db 'Loading...', ENDL, 0
msg_read_failed: db 'Read from disk failed', ENDL, 0
msg_stage2_not_found: db 'STAGE2.BIN file not found', ENDL, 0
file_stage2_bin: db 'STAGE2  BIN'

have_extensions: db 0
extensions_dap:
    .size db 0x10
    .unused db 0
    .count: dw 0
    .segment: dw 0
    .offset: dw 0
    .lba dq 0

; for protected mode 
STAGE2_LOAD_SEGMENT equ 0x0
STAGE2_LOAD_OFFSET equ 0x500

times 510-30-($-$$) db 0

stage2_location: times 30 db 0

dw 0xAA55

buffer:
