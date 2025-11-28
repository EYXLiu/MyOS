#include <stdint.h>
#include "stdio.h"
#include "memory.h"
#include <hal/hal.h>
#include <arch/i686/irq.h>
#include "debug.h"

extern uint8_t __bss_start;
extern uint8_t __end;

void __attribute__((section(".entry"))) kstart(uint16_t bootDrive) 
{
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();

    printf("Hello world from kernel\n");

    log_debug("Main", "Hello from debug");
    log_info("Main", "Hello from info");
    log_warn("Main", "Hello from warn");
    log_err("Main", "Hello from err");
    log_crit("Main", "Hello from crit");

end:
    for (;;);
}
