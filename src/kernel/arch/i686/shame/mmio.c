#include "mmio.h"

#define PAGE_SIZE 4096
#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_MMIO 0x10

uint32_t i686_MMIO_Read32(volatile void* addr) {
    return *(volatile uint32_t*)addr;
}

void i686_MMIO_Write32(volatile void* addr, uint32_t value) {
    *(volatile uint32_t*)addr = value;
}
