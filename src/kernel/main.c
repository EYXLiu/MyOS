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
#include <kfat/fat.h>
#include <arch/i686/page.h>
#include <arch/i686/ide.h>

extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) kstart(BootParams* bootParams) 
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    BlockMem_Initialize(&__end);

    //i686_Page_Initialize();

    uint16_t buffer[256];
    for (int i = 0; i < 256; i++) buffer[i] = i;
    i686_IDE_Write(0, buffer);
    for (int i = 0; i < 256; i++) buffer[i] = 0;
    i686_IDE_Read(0, buffer);
    printf("First 16 bytes of sector 0:\n");
    for (int i = 0; i < 8; i++) {
        printf("%u ", buffer[i]);
    }
    printf("\n");

    goto end;

    ShellInitialize();
    ShellRun();

end:
    for (;;);
}
