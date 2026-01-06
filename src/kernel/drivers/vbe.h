#pragma once

#include <boot/bootparams.h>
#include <kfat/fat.h>

void VBE_Initialize(VbeInfoBlock* vbeInfo, VbeModeInfo* vbeMode);
void VBE_SetBG(Directory* dir, const char* s);

void VBE_PutP(uint32_t x, uint32_t y, uint32_t color);
uint32_t BGRA(uint8_t r, uint8_t g, uint8_t b);
