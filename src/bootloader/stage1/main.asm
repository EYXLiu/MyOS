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

    ; read drive parameters instead of relying on data
    push es
    mov ah, 0x08
    int 0x13
    jc floppy_error
    pop es

    and cl, 0x3F
    xor ch, ch
    mov [bdb_sectors_pt], cx

    inc dh
    mov [bdb_heads], dh

    ; read FAT root directory
    mov ax, [bdb_sectors_pf]
    mov bl, [bdb_fat_count]
    xor bh, bh
    mul bx
    add ax, [bdb_reserved]
    push ax

    mov ax, [bdb_sectors_pf]
    shl ax, 5
    xor dx, dx
    div word [bdb_bytes_ps]

    test dx, dx
    jz .root_dir_else
    inc ax

.root_dir_else:
    ; read root directory
    mov cl, al
    pop ax
    mov dl, [ebr_driver_number]
    mov bx, buffer
    call disk_read

    ; search for stage2.bin
    xor bx, bx
    mov di, buffer

.search_stage2:
    mov si, file_stage2_bin
    mov cx, 11
    push di
    repe cmpsb  ; compare ds:si and es:di while zero flag = 1 (they're equal)
    pop di
    je .found_stage2

    add di, 32
    inc bx
    cmp bx, [bdb_dir_entry]
    jl .search_stage2
.found_stage2:
    ; di now has address to entry (offset 26)
    mov ax, [di + 26]
    mov [stage2_cluster], ax

    ; load fat from memory to disk
    mov ax, [bdb_reserved]
    mov bx, buffer
    mov cl, [bdb_sectors_pf]
    mov dl, [ebr_driver_number]
    call disk_read

    ; read stage2 and process FAT chain
    ; find maximum location on memory map to use
    mov bx, STAGE2_LOAD_SEGMENT
    mov es, bx
    mov bx, STAGE2_LOAD_OFFSET

.load_stage2_loop:
    ; read cluster
    mov ax, [stage2_cluster]
    ; hardcoded, change
    add ax, 31

    ; care if file is bigger than 64kb
    mov cl, 1
    mov dl, [ebr_driver_number]
    call disk_read

    add bx, [bdb_bytes_ps]
    mov ax, [stage2_cluster]
    mov cx, 3
    mul cx
    mov cx, 2
    div cx

    mov si, buffer
    add si, ax
    mov ax, [ds:si]

    or dx, dx
    jz .even

.odd: 
    shr ax, 4
    jmp .next_cluster_after
.even:
    and ax, 0x0FFF;
.next_cluster_after:
    cmp ax, 0x0FF8
    jae .read_finish
    mov [stage2_cluster], ax
    jmp .load_stage2_loop

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

.done:
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


msg_loading: db 'Loading...', ENDL, 0
msg_read_failed: db 'Read from disk failed', ENDL, 0
msg_stage2_not_found: db 'STAGE2.BIN file not found', ENDL, 0
file_stage2_bin: db 'STAGE2  BIN'
stage2_cluster: dw 0

; for protected mode 
STAGE2_LOAD_SEGMENT equ 0x0
STAGE2_LOAD_OFFSET equ 0x500

times 510-($-$$) db 0
dw 0xAA55

buffer: