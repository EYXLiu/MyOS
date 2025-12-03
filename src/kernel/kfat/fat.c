#include "fat.h"
#include <debug.h>

DISK* g_Disk;

FAT_Data* g_Data;
uint8_t* g_Fat = NULL;

uint32_t g_FirstFatSector;
uint32_t g_RootDirSector;
uint32_t g_RootDirSectors;
uint32_t g_DataSectionLba;

void FAT_Initialize(FatInfo fatInfo, DISK* disk) {
    g_Disk = disk;
    g_Data = (FAT_Data*)fatInfo.FatAddress;
    g_Fat = (uint8_t*)g_Data+ sizeof(FAT_Data);
    log_debug("FAT", "g_Data: %u, g_Fat: %u", g_Data, g_Fat);

    FAT_BootSector* bs = &g_Data->BS.BootSector;
    g_FirstFatSector = bs->ReservedSectors;
    g_RootDirSector = bs->ReservedSectors + (bs->FatCount * bs->SectorsPerFat);
    g_RootDirSectors = ((bs->DirEntryCount * 32) + (bs->BytesPerSector - 1)) / bs->BytesPerSector;

    log_debug("FAT", "g_FirstFatSector: %u, g_RootDirSector: %u, g_RootDirSectors: %u", g_FirstFatSector, g_RootDirSector, g_RootDirSectors);

    g_DataSectionLba = g_RootDirSector + g_RootDirSectors;

    log_debug("FAT", "g_DataSectionLba: %u", g_DataSectionLba);
}

void FAT_LbaToChs(uint32_t lba, uint16_t* cylinderOut, uint16_t* headOut, uint16_t* sectorOut) {
    // sector = (LBA % sectors per track + 1)
    *sectorOut = lba % g_Disk->sectors + 1;
    // cylinder = (LBA / sectors per track) / heads
    *cylinderOut = (lba / g_Disk->sectors) / g_Disk->heads;
    // head = (LBA / sectors per track) % heads
    *headOut = (lba / g_Disk->sectors) % g_Disk->heads;
}
