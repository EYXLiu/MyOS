#include "fat.h"

#include <stddef.h>
#include <stdio.h>
#include <debug.h>
#include <arch/i686/fdc.h>

void FAT_ReadSectors(uint32_t lba, uint32_t count, uint8_t* buffer) {
    printf("count: %u\n", count);
    for (uint32_t i = 0; i < count; i++) {
        uint16_t cylinder, head, sector;
        FAT_LbaToChs(lba + i, &cylinder, &head, &sector);
        printf("FAT: cylinder: %u, head:%u, sector:%u\n", cylinder, head, sector);
    }
}

uint32_t FAT_ClusterToLba(uint32_t cluster) {
    return g_DataSectionLba + (cluster - 2) * g_Data->BS.BootSector.SectorsPerCluster;
}

void FAT_ConvertName(FAT_DirectoryEntry* entry, char* buffer) {
    int idx = 0;

    for (int i = 0; i < 8 && entry->Name[i] != ' '; i++) {
        buffer[idx++] = entry->Name[i];
    }

    if (entry->Name[8] != ' ') {
        buffer[idx++] = '.';
        for (int i = 8; i < 11 && entry->Name[i] != ' '; i++)
            buffer[idx++] = entry->Name[i];
    }

    buffer[idx] = 0;
}

uint32_t FAT_NextCluster(uint32_t currentCluster) {    
    uint32_t fatIndex = currentCluster * 3 / 2;

    uint32_t result;

    if (currentCluster % 2 == 0)
        result = (*(uint16_t*)(g_Fat + fatIndex)) & 0x0FFF;
    else
        result = ((*(uint16_t*)(g_Fat + fatIndex)) >> 4) & 0x0FFF;
    
    if (result >= 0xFF8) 
        return 0;
    
    return result;
}

void FAT_LsRoot() {
    FAT_BootSector* bs = &g_Data->BS.BootSector;
    static uint8_t buffer[8192];
    FAT_ReadSectors(g_RootDirSector, g_RootDirSectors, buffer);
    FAT_DirectoryEntry* entries = (FAT_DirectoryEntry*)buffer;

    for (int i = 0; i < bs->DirEntryCount; i++) {
        FAT_DirectoryEntry* e = &entries[i];

        if (e->Name[0] == 0x00) break;
        if (e->Name[0] == 0xE5) continue;
        if (e->Attributes == FAT_ATTRIBUTE_LFN) continue;

        char name[13];
        FAT_ConvertName(e, name);

        printf("%s ", name);
    }
    printf("\n");
}

void FAT_LsDir(uint32_t cluster) {
    FAT_BootSector* bs = &g_Data->BS.BootSector;
}

void FAT_Ls() {
    FAT_LsRoot();
}
