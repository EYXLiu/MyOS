#pragma once

#include <stdint.h>

typedef enum
{
    IDT_FLAG_GATE_TASK = 0x5,
    IDT_FLAG_GATE_16BIT_INT = 0x6, // interrupt will just go to next instruction
    IDT_FLAG_GATE_16BIT_TRAP = 0x7, // trap will retry
    IDT_FLAG_GATE_32BIT_INT = 0xE,
    IDT_FLAG_GATE_32BIT_TRAP = 0xF,

    IDT_FLAG_RING0 = (0 << 5), // privilege levels
    IDT_FLAG_RING1 = (1 << 5),
    IDT_FLAG_RING2 = (2 << 5),
    IDT_FLAG_RING3 = (3 << 5),

    IDT_FLAG_PRESENT = 0x80, // enabled or disabled

} IDT_FLAGS;

void i686_IDT_Initialize();
void i686_IDT_SetGate(int interrupt, void* base, uint16_t segmentDescriptor, uint8_t flags);
void i686_IDT_EnableGate(int interrupt);
void i686_IDT_DisableGate(int interrupt);
