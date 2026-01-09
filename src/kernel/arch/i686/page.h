#pragma once

#include <stdint.h>
#include <stdbool.h>

#define PAGE_PRESENT 0x1
#define PAGE_RW 0x2
#define PAGE_PCD 0x10
#define PAGE_PWT 0x8

typedef struct {
    bool present;
    uint32_t phys_addr;
    uint32_t swap_idx;
} Page;

void i686_Page_Initialize();
void i686_Page_Map(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);
void i686_Page_MMIO(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);

void __attribute__((cdecl)) i686_Page_Directory_Load(uint32_t pd_addr);
void __attribute__((cdecl)) i686_Page_Enable();
void i686_Flush_Tlb(uint32_t addr);
