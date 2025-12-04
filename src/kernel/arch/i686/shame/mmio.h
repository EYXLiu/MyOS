#pragma once

#include <stdint.h>
#include <stddef.h>

uint32_t i686_MMIO_Read32(volatile void* addr);
void i686_MMIO_Write32(volatile void* addr, uint32_t value);
void* mmio_map(uintptr_t phys_addr, size_t size);
