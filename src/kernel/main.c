#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>
#include "debug.h"
#include <boot/bootparams.h>
#include <mem/block_allocator.h>
#include <shell/shell.h>
#include <fat12/fat.h>
#include <arch/i686/page.h>
#include <arch/i686/ide.h>
#include <kfat/fat.h>
#include <string.h>
#include <arch/i686/pmm.h>
#include <drivers/rtl8139.h>

extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) kstart(BootParams* bootParams) 
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    BlockMem_Initialize(&__end);

    i686_Page_Initialize();

    i686_PMM_Initialize((uintptr_t)&__end + HEAP_SIZE);

    RTL8139_Initialize();

goto end;

    uint32_t root = FS_Initialize();
    Directory dir;
    FS_SetDirectory(&dir, root);

    Shell_Initialize(&dir);
    Shell_Run();

end:
    for (;;);
}
