#pragma once

#include <stdint.h>
#include <stddef.h>
#include <fat12/fat.h>
#include <boot/bootparams.h>

extern DISK* g_Disk;

extern FAT_Data* g_Data;
extern uint8_t* g_Fat;

extern uint32_t g_FirstFatSector;
extern uint32_t g_RootDirSector;
extern uint32_t g_RootDirSectors;
extern uint32_t g_DataSectionLba;

void FAT_Initialize(FatInfo fatInfo, DISK* disk);
void FAT_LbaToChs(uint32_t lba, uint16_t* cylinderOut, uint16_t* sectorOut, uint16_t* headOut);
void FAT_Ls();

