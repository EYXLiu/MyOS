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

extern uint8_t __bss_start;
extern uint8_t __end;

static Directory g_Dir;

void __attribute__((section(".entry"))) kstart(BootParams* bootParams) 
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    BlockMem_Initialize(&__end);

    //i686_Page_Initialize();

    int root = FS_Initialize();
    FS_SetDirectory(&g_Dir, root);
    FS_DirCreate(&g_Dir, "test");
    FS_DirCreate(&g_Dir, "test2");
    FS_DirDelete(&g_Dir, 1);
    FS_DirCreate(&g_Dir, "test3");
    FS_LS(&g_Dir);

    goto end;


    ShellInitialize();
    ShellRun();

end:
    for (;;);
}
