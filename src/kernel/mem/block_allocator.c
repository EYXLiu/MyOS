#include "block_allocator.h"
#include <stdint.h>
#include <stdio.h>
#include <debug.h>

struct BlockHeader* g_BlockHead;

#define HEAP_SIZE (16 * 1024 * 1024) // 16mb

uintptr_t g_HeapBegin;
uintptr_t g_HeapEnd;

void BlockMem_Initialize(uint8_t* __end) {
    g_HeapBegin = (uintptr_t)__end;
    g_HeapEnd = g_HeapBegin + HEAP_SIZE;

    struct BlockHeader* initial = (struct BlockHeader*)g_HeapBegin;
    initial->size = HEAP_SIZE - sizeof(struct BlockHeader);
    initial->free = true;
    initial->next = NULL;
    initial->prev = NULL;
    g_BlockHead = initial;

    log_debug("HEAP", "Heap allocated at 0x%x-0x%x", g_HeapBegin, g_HeapEnd);
}

void* KMalloc(size_t size) {
    struct BlockHeader* current = g_BlockHead;

    while (current) {
        if (current->free && current->size >= size) {
            if (current->size > size + sizeof(struct BlockHeader)) {
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
        block->size += sizeof(struct BlockHeader) + block->next->size;
        block->next = block->next->next;
        if (block->next) block->next->prev = block;
    }
    if (block->prev && block->prev->free) {
        block->prev->size += sizeof(struct BlockHeader) + block->size;
        block->prev->next = block->next;
        if (block->next) block->next->prev = block->prev;
    }
}
