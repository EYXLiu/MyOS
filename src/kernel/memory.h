#pragma once

#include <stdint.h>

int memcmp(const void* ptr1, const void* ptr2, uint16_t size);
void* memcpy(void* dst, const void* src, uint16_t size);
void* memset(void * ptr, int value, uint16_t size);
