#include "page.h"
#include <stdio.h>
#include <arch/i686/pmm.h>

uint32_t g_PageDirectory[1024] __attribute__((aligned(4096)));
uint32_t g_FirstPageTable[1024] __attribute__((aligned(4096)));

void i686_Page_Initialize() {
    for (int i = 0; i < 1024; i++)
        g_FirstPageTable[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_RW;
    
    g_PageDirectory[0] = ((uint32_t)g_FirstPageTable) | PAGE_PRESENT | PAGE_RW;
    for (int i = 1; i < 1024; i++)
        g_PageDirectory[i] = 0;
    
    i686_Page_Enable((uint32_t)g_PageDirectory);
}

void i686_Page_Map(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags) {
    uint32_t pd_idx = virtual_addr >> 22;
    uint32_t pt_idx = (virtual_addr >> 12) & 0x3FF;
    uint32_t* page_table;

    if (pd_idx == 0) {
        page_table = g_FirstPageTable;
    } else {
        if (!(g_PageDirectory[pd_idx] & PAGE_PRESENT)) {
            uint32_t pt_phys = i686_PMM_AllocPage();
            page_table = (uint32_t*)(pt_phys);
            for (int i = 0; i < 1024; i++) page_table[i] = 0;
            g_PageDirectory[pd_idx] = pt_phys | PAGE_PRESENT | PAGE_RW;
        } else {
            page_table = (uint32_t*)(g_PageDirectory[pd_idx] & 0xFFFFF000);
        }
    }
    page_table[pt_idx] = (physical_addr & 0xFFFFF000) | (flags & 0xFFF) | PAGE_PRESENT;
}

void i686_Page_MMIO(uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags) {
    i686_Page_Map(virtual_addr, physical_addr, flags | PAGE_PCD | PAGE_PWT);
}
