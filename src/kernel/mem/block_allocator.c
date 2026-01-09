#include "block_allocator.h"
#include <stdint.h>
#include <stdio.h>
#include <debug.h>
#include <arch/i686/page.h>
#include <arch/i686/pmm.h>

struct BlockHeader* g_BlockHead;

uintptr_t g_HeapBegin;
uintptr_t g_HeapEnd;

#define MEM_VIRT 0xC0400000

void BlockMem_Initialize() {
    g_HeapBegin = (uintptr_t)MEM_VIRT;
    g_HeapEnd = g_HeapBegin + HEAP_SIZE;

    uintptr_t addr;
    for (addr = g_HeapBegin; addr < g_HeapEnd; addr += PAGE_SIZE) {
        uint32_t phys = i686_PMM_AllocPage();
        if (phys == 0xFFFFFFFF) {
            log_debug("HEAP", "out of physical memory");
            return;
        }
        i686_Page_Map(addr, phys, PAGE_PRESENT | PAGE_RW);
    }

    struct BlockHeader* initial = (struct BlockHeader*)g_HeapBegin;
    initial->size = HEAP_SIZE - sizeof(struct BlockHeader);
    initial->free = true;
    initial->next = NULL;
    initial->prev = NULL;
    g_BlockHead = initial;

    log_debug("HEAP", "Heap allocated at 0x%x-0x%x", g_HeapBegin, g_HeapEnd);
}

void* KMalloc(size_t size) {
    size = (size + 7) & ~7;

    struct BlockHeader* current = g_BlockHead;

    while (current) {
        if (current->free && current->size >= size) {
            if (current->size >= size + sizeof(struct BlockHeader) + 8) {
                struct BlockHeader* newBlock = (void*)((uint8_t*)current + sizeof(struct BlockHeader) + size);
                newBlock->size = current->size - size - sizeof(struct BlockHeader);
                newBlock->free = true;
                newBlock->next = current->next;
                newBlock->prev = current;
                
                if (current->next) current->next->prev = newBlock;
                
                current->next = newBlock;
                current->size = size;
            }
            current->free = false;
            return (void*)((uint8_t*)current + sizeof(struct BlockHeader));
        }
        current = current->next;
    }
    printf("Failed to kmalloc size %i\n", size);
    return NULL;
}

void KFree(void* ptr) {
    if (!ptr) return;

    struct BlockHeader* block = (struct BlockHeader*)((uint8_t*)ptr - sizeof(struct BlockHeader));
    block->free = 1;

    if (block->next && block->next->free) {
        struct BlockHeader* prev = block->prev;
        block->size += sizeof(struct BlockHeader) + block->next->size;
        if (block->next) block->next = block->next->next;
        if (block->next) block->next->prev = block;
        block = prev;
    }
    if (block && block->prev && block->prev->free) {
        block->prev->size += sizeof(struct BlockHeader) + block->size;
        block->prev->next = block->next;
        if (block->next) block->next->prev = block->prev;
    }
}
