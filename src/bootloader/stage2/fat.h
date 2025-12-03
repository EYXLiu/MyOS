#pragma once

#include "stdint.h"
#include "disk.h"
#include <fat12/fat.h>
#include <boot/bootparams.h>

bool FAT_Initialize(DISK* disk);
FAT_File* FAT_Open(DISK* disk, const char* path);
uint32_t FAT_Read(DISK* disk, FAT_File* file, uint32_t byteCount, void* dataOut);
bool FAT_ReadEntry(DISK* disk, FAT_File* file, FAT_DirectoryEntry* dirEntry);
void FAT_Close(FAT_File* file);

FatInfo FAT_GetParams();
