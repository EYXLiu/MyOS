#include "isr.h"
#include "idt.h"
#include "gdt.h"
#include "io.h"
#include <stdio.h>
#include <stddef.h>
#include <debug.h>

ISRHandler g_ISRHandlers[256];

static const char* const g_Exceptions[] = {
    "Divide by zero error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Control Protection Exception ",
    "",
    "",
    "",
    "",
    "",
    "",
    "Hypervisor Injection Exception",
    "VMM Communication Exception",
    "Security Exception",
    ""
};

void i686_ISR_InitializeGates();

void i686_ISR_Initialize() {
    i686_ISR_InitializeGates();
    for (int i = 0; i < 256; i++)
        i686_IDT_EnableGate(i);

    i686_IDT_DisableGate(0x80);
}

void __attribute__((cdecl)) i686_ISR_Handler(Registers* regs) {
    if (g_ISRHandlers[regs->interrupt] != NULL)
        g_ISRHandlers[regs->interrupt](regs);
    
    else if (regs->interrupt >= 32)
        log_err("ISR", "unhandled ISR %d", regs->interrupt);
    
    else {
        log_err("ISR", "unhandled exception %d %s", regs->interrupt, g_Exceptions[regs->interrupt]);
        
        log_err("ISR", "  eax=%x  ebx=%x  ecx=%x  edx=%x  esi=%x  edi=%x",
               regs->eax, regs->ebx, regs->ecx, regs->edx, regs->esi, regs->edi);

        log_err("ISR", "  esp=%x  ebp=%x  eip=%x  eflags=%x  cs=%x  ds=%x  ss=%x",
               regs->esp, regs->ebp, regs->eip, regs->eflags, regs->cs, regs->ds, regs->ss);

        log_err("ISR", "  interrupt=%x  errorcode=%x", regs->interrupt, regs->error);
        
        uint32_t fault_addr;
        __asm__ volatile("mov %%cr2, %0": "=r"(fault_addr));
        log_err("PAGE_FAULT", "Fault at 0x%x", fault_addr);

        printf("Fatal exception occured; system stopping\n");
        i686_Halt();
    }
}

void i686_ISR_RegisterHandler(int interrupt, ISRHandler handler) {
    g_ISRHandlers[interrupt] = handler;
    i686_IDT_EnableGate(interrupt);
}
