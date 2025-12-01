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

void BlockMemInitialize(uint8_t* __end);
void* KMalloc(size_t size);
void KFree(void* ptr);
