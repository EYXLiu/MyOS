#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <boot/bootparams.h>

bool VBE_GetControllerInfo(VbeInfoBlock* info);
bool VBE_GetModeInfo(uint16_t mode, VbeModeInfo* info);
bool VBE_SetMode(uint16_t mode);
