#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>
#include "debug.h"
#include <boot/bootparams.h>
#include <mem/block_allocator.h>

extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) kstart(BootParams* bootParams) 
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    printf("Hello world from kernel\n");

    BlockMemInitialize(&__end);

    void* a = KMalloc(64);
    void* b = KMalloc(128);
    void* c = KMalloc(32);

    printf("Allocated a = 0x%x\n", a);
    printf("Allocated b = 0x%x\n", b);
    printf("Allocated c = 0x%x\n", c);

    for (int i = 0; i < 64; i++) ((uint8_t*)a)[i] = i;
    for (int i = 0; i < 128; i++) ((uint8_t*)b)[i] = i * 2;
    for (int i = 0; i < 32; i++) ((uint8_t*)c)[i] = i + 1;

    KFree(b);
    KFree(a);

    void* d = KMalloc(100);
    printf("Allocated d = 0x%x\n", d);

end:
    for (;;);
}
