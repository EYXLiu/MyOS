#include "pmm.h"
#include <arch/i686/page.h>
#include <debug.h>

static uint8_t* bitmap;

void i686_PMM_SetBit(uint32_t bit) {
    bitmap[bit / 8] |= (1 << (bit % 8));
}

void i686_PMM_ClearBit(uint32_t bit) {
    bitmap[bit / 8] &= ~(1 << (bit % 8));
}

uint8_t i686_PMM_TestBit(uint32_t bit) {
    return bitmap[bit / 8] & (1 << (bit % 8));
}

void i686_PMM_Initialize(uint32_t bitmap_addr) {
    uintptr_t bitmap_start = (bitmap_addr + 0xFFF) & ~0xFFF;
    size_t bitmap_size = NUM_PAGES / 8;
    size_t pages_needed = (bitmap_size + 0xFFF) / 0x1000;

    for (size_t i = 0; i < pages_needed; i++) 
        i686_Page_Map(bitmap_start + i*0x1000, bitmap_start + i*0x1000, PAGE_PRESENT | PAGE_RW);

    bitmap = (uint8_t*)bitmap_start;

    uintptr_t bitmap_end = bitmap_start + bitmap_size;
    uint32_t first_free_page = (bitmap_end + 0xFFF) / 4096;

    for (uint32_t i = 0; i < first_free_page; i++)
        i686_PMM_SetBit(i);
    for (uint32_t i = first_free_page; i < NUM_PAGES; i++)
        i686_PMM_ClearBit(i);
    
    log_debug("PMM", "Pmm allocated at 0x%x-0x%x", bitmap_start, bitmap_end);
}

uint32_t i686_PMM_AllocPage() {
    for (int i = 0; i < NUM_PAGES; i++) {
        if (!i686_PMM_TestBit(i)) {
            i686_PMM_SetBit(i);
            return i * 4096;
        }
    }
    return 0;
}

void i686_PMM_FreePage(uint32_t phys_addr) {
    uint32_t index = phys_addr / 4096;
    i686_PMM_ClearBit(index);
}
