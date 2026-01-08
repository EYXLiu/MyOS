;
; i686_Page_Directory_Load(uint32_t pd_addr)
;
global i686_Page_Directory_Load
i686_Page_Directory_Load:
    push ebp
    mov ebp, esp
    mov eax, [esp + 8] 
    mov cr3, eax
    mov esp, ebp
    pop ebp
    ret

;
; i686_Page_Enable()
;
global i686_Page_Enable
i686_Page_Enable:
    push ebp
    mov ebp, esp
    mov eax, cr0
    or eax, 0x80010000
    mov cr0, eax
    mov esp, ebp
    pop ebp
    ret
