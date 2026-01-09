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
#include <drivers/vbe.h>
#include <drivers/gpu.h>
#include <kstring/string.h>

extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) kstart(BootParams* bootParams) {
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    for (int i = 0; i < bootParams->Memory.RegionCount; i++) {
        log_debug("K", "Type %x, length %llx, begin %llx", bootParams->Memory.Regions[i].Type, bootParams->Memory.Regions[i].Length, bootParams->Memory.Regions[i].Begin);
    }


    log_debug("K", "Start 0x%x, end 0x%x", &__bss_start, &__end);

    HAL_Initialize();

    i686_PMM_Initialize((uintptr_t)&__end, &bootParams->Memory);

    i686_Page_Initialize();

    // initialize first because framebuffer is required memory
    VBE_Initialize(bootParams->vbeInfo, bootParams->vbeMode);
    
    uint32_t root = FS_Load();
    Directory dir;
    FS_SetDirectory(&dir, root);

    VBE_SetBG(&dir, "bgr.bin");

    BlockMem_Initialize();

    VBE_SetBG(&dir, "bgr2.bin");

goto end;
    GPU_Initialize();
    RTL8139_Initialize();

    Shell_Initialize(&dir);
    Shell_Run();

end:
    for (;;);
}
