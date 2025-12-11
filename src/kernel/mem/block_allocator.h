#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

struct BlockHeader {
    size_t size;
    struct BlockHeader* next;
    struct BlockHeader* prev;
    bool free;
};

#define HEAP_SIZE (16 * 1024 * 1024) // 16mb

void BlockMem_Initialize(uint8_t* __end);
void* KMalloc(size_t size);
void KFree(void* ptr);
