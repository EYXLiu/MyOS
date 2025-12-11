#pragma once

#include <stdint.h>
#include <stddef.h>

#define BITMAP_SIZE (64 * 1024 * 1024) // 16mb
#define NUM_PAGES (BITMAP_SIZE / 4096)

void i686_PMM_Initialize(uint32_t bitmap_addr);
uint32_t i686_PMM_AllocPage();
void i686_PMM_FreePage(uint32_t phys_addr);
