;
;
;
global i686_Page_Enable
i686_Page_Enable:
    mov cr3, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret
