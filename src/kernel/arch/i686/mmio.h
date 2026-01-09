#pragma once

#include <stdint.h>

uint32_t i686_MMIO_Read32(uintptr_t addr);
void i686_MMIO_Write32(uintptr_t addr, uint32_t value);
