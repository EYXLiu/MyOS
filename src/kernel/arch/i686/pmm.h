#pragma once

#include <stdint.h>
#include <stddef.h>
#include <boot/bootparams.h>

#define PAGE_SIZE 4096

void i686_PMM_Initialize(uint32_t bitmap_addr, MemoryInfo* memInfo);
uint32_t i686_PMM_AllocPage();
void i686_PMM_FreePage(uint32_t phys_addr);
