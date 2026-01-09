#include "mmio.h"

uint32_t i686_MMIO_Read32(uintptr_t addr) {
    return *((volatile uint32_t*)addr);
}

void i686_MMIO_Write32(uintptr_t addr, uint32_t value) {
    *((volatile uint32_t*)addr) = value;
}
