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

    uint32_t root = FS_Initialize();
    FS_SetDirectory(&g_Dir, root);

    printf("File create testfile\n");
    FS_FileCreate(&g_Dir, "testfile");
    printf("File write testfile hello\n");
    FS_FileWrite(&g_Dir, "testfile", "hello", 5);
    printf("File LS\n");
    FS_LS(&g_Dir);
    printf("File read testfile\n");
    FS_FilePrint(&g_Dir, "testfile");

    printf("File append testfile hello\n");
    FS_FileAppend(&g_Dir, "testfile", "hello", 5);
    printf("File read testfile\n");
    FS_FilePrint(&g_Dir, "testfile");
    printf("File done\n");
    FS_LS(&g_Dir);

    goto end;


    ShellInitialize();
    ShellRun();

end:
    for (;;);
}
