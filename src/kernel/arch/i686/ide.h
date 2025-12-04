#pragma once

#include <stdint.h>

void i686_IDE_Read(uint32_t lba, uint16_t* buffer);
void i686_IDE_Write(uint32_t lba, const uint16_t* buffer);
