#include "pmm.h"
#include <arch/i686/page.h>
#include <debug.h>

uint8_t* bitmap;
uint32_t total_pages;

void i686_PMM_SetBit(uint32_t bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

void i686_PMM_ClearBit(uint32_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

uint8_t i686_PMM_TestBit(uint32_t bit) {
    return (bitmap[bit / 8] >> (bit % 8)) & 1;
}

void i686_PMM_Initialize(uint32_t bitmap_addr, MemoryInfo* memInfo) {
    // bitmap needs to be page aligned
    uintptr_t bitmap_start = (bitmap_addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

    // find number of pages
    total_pages = 0;
    for (int i = 0; i < memInfo->RegionCount; i++) {
        MemoryRegion* memRegion = &memInfo->Regions[i];

        uintptr_t start = (memRegion->Begin + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
        uintptr_t end = start + memRegion->Length;

        total_pages += (end - start) / PAGE_SIZE;
    }

    size_t bitmap_size = (total_pages + 7) / 8;
    bitmap = (uint8_t*)bitmap_start;
    uintptr_t bitmap_end = bitmap_start + bitmap_size;

    for (size_t i = 0; i < total_pages; i++)
        i686_PMM_SetBit(i);
    
    for (int i = 0; i < memInfo->RegionCount; i++) {
        MemoryRegion* memRegion = &memInfo->Regions[i];
        if (memRegion->Type != 0x1) continue;

        uintptr_t start = (memRegion->Begin + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
        uintptr_t end = start + memRegion->Length;

        for (uintptr_t addr = start; addr < end; addr += PAGE_SIZE) {
            if (addr < bitmap_end) continue;
            i686_PMM_ClearBit(addr / PAGE_SIZE);
        }
    }

    log_debug("PMM", "bitmap allocated at 0x%x-0x%x, managing %u pages", bitmap_start, bitmap_end, total_pages);
}

uint32_t i686_PMM_AllocPage() {
    for (int i = 0; i < total_pages; i++) {
        if (!i686_PMM_TestBit(i)) {
            i686_PMM_SetBit(i);
            return i * PAGE_SIZE;
        }
    }
    return 0xFFFFFFFF;
}

void i686_PMM_FreePage(uint32_t phys_addr) {
    uint32_t index = phys_addr / 4096;
    i686_PMM_ClearBit(index);
}
