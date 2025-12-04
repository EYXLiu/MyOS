#pragma once

#include <stdint.h>

#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_PCD 0x10
#define PAGE_PWT 0x8

void i686_Page_Initialize();
void i686_Page_Map(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
void i686_Page_MMIO(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);

void __attribute__((cdecl)) i686_Page_Enable(uint32_t pd_addr);
