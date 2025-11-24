#include "memory.h"

int memcmp(const void far* ptr1, const void far* ptr2, uint16_t size) {
    const uint8_t far* u8Ptr1 = (const uint8_t far *)ptr1;
    const uint8_t far* u8Ptr2 = (const uint8_t far *)ptr2;

    for (uint16_t i = 0; i < size; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}

void far* memcpy(void far* dst, const void far* src, uint16_t size) {
    uint8_t far* u8Dst = (uint8_t far*)dst;
    const uint8_t far* u8Src = (const uint8_t far*)src;

    for (uint16_t i = 0; i < size; i++)
        u8Dst[i] = u8Src[i];
    
    return dst;
}

void far* memset(void far* ptr, int value, uint16_t size) {
    uint8_t far* u8Ptr= (uint8_t far*)ptr;

    for (uint16_t i = 0; i < size; i++)
        u8Ptr[i] = (uint8_t)value;
    
    return ptr;
}
