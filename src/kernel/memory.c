#include "memory.h"

int memcmp(const void* ptr1, const void* ptr2, uint16_t size) {
    const uint8_t* u8Ptr1 = (const uint8_t *)ptr1;
    const uint8_t* u8Ptr2 = (const uint8_t *)ptr2;

    for (uint16_t i = 0; i < size; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}

void* memcpy(void* dst, const void* src, uint16_t size) {
    uint8_t* u8Dst = (uint8_t*)dst;
    const uint8_t* u8Src = (const uint8_t*)src;

    for (uint16_t i = 0; i < size; i++)
        u8Dst[i] = u8Src[i];
    
    return dst;
}

void* memset(void* ptr, int value, uint16_t size) {
    uint8_t* u8Ptr= (uint8_t*)ptr;

    for (uint16_t i = 0; i < size; i++)
        u8Ptr[i] = (uint8_t)value;
    
    return ptr;
}
