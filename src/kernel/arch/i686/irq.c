#include "irq.h"
#include "pic.h"
#include "i8259.h"
#include "io.h"
#include <stddef.h>
#include <stdio.h>
#include <util/array.h>

#define PIC_REMAP_OFFSET 0x20

IRQHandler g_IRQHandlers[16];
static const PICDriver* g_PicDriver = NULL;

void i686_IRQ_Handler(Registers* regs) {
    int irq = regs->interrupt - PIC_REMAP_OFFSET;
    
    if (g_IRQHandlers[irq] != NULL) {
        g_IRQHandlers[irq](regs);
    } else {
        printf("Unhandled IRQ %d\n", irq);
    }

    g_PicDriver->SendEndOfInterrupt(irq);
}

void i686_IRQ_Initialize() {
    const PICDriver* picDrivers[] = {
        i8259_GetDriver(),
    };

    for (int i = 0; i < SIZE(picDrivers); i++)
        if (picDrivers[i]->Probe())
            g_PicDriver = picDrivers[i];
    
    if (g_PicDriver == NULL) {
        printf("IRQ: warning, no pic driver\n");
        return;
    }

    g_PicDriver->Initialize(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);

    // register ISR for each IRQ
    for (int i = 0; i < 16; i++)
        i686_ISR_RegisterHandler(PIC_REMAP_OFFSET + i, i686_IRQ_Handler);
    
    i686_EnableInterrupts();
}

void i686_IRQ_RegisterHandler(int irq, IRQHandler handler) {
    g_IRQHandlers[irq] = handler;
    g_PicDriver->Unmask(irq);
}
