#pragma once

#include <boot/bootparams.h>
#include <kfat/fat.h>

void VBE_Initialize(VbeInfoBlock* vbeInfo, VbeModeInfo* vbeMode);
void VBE_SetBG(Directory* dir, const char* s);
