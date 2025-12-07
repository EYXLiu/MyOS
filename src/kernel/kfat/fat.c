#include "fat.h"
#include <memory.h>
#include <stdio.h>
#include <arch/i686/ide.h>
#include <string.h>
#include <stdbool.h>

static Superblock g_Superblock;
static uint8_t g_Bat[FS_BAT_BLOCKS * FS_BLOCK_SIZE];

void FS_ReadBlock(uint32_t block, void* buffer) {
    i686_IDE_Read(block, (uint8_t*)buffer);
}

void FS_WriteBlock(uint32_t block, const void* buffer) {
    i686_IDE_Write(block, (const uint8_t*)buffer);
}

void FS_BatSet(uint32_t block) {
    g_Bat[block / 8] |= (1 << (block % 8));
}

void FS_BatClear(uint32_t block) {
    g_Bat[block / 8] &= ~(1 << (block % 8));
}

bool FS_BatIsSet(uint32_t block) {
    return g_Bat[block / 8] & (1 << (block % 8));
}

int FS_BatFindFreeBlock() {
    uint32_t i = 0;
    while (FS_BatIsSet(i) && i != 0xFFFFFFFF) i++;
    return i;
}

void FS_SaveBat(uint32_t block) {
    uint32_t sector = block / 4096;
    uint32_t lba = g_Superblock.bat_start_block + sector;
    void* bat_sector = g_Bat + (sector * 512);

    FS_WriteBlock(lba, bat_sector);
}

void FS_SetDirectory(Directory* emptyDir, uint32_t block) {
    uint8_t buffer[FS_BLOCK_SIZE];
    FS_ReadBlock(block, buffer);
    memcpy(emptyDir, buffer, sizeof(Directory));
}

uint32_t FS_Initialize() {
    uint8_t buffer[FS_BLOCK_SIZE];

    g_Superblock.magic = FS_MAGIC;
    g_Superblock.total_blocks = FS_TOTAL_BLOCKS;
    g_Superblock.bat_start_block = FS_BAT_LBA;
    g_Superblock.bat_block_count = FS_BAT_BLOCKS;
    g_Superblock.root_dir_block = FS_DATA_LBA;

    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &g_Superblock, sizeof(Superblock));

    FS_WriteBlock(FS_SUPERBLOCK_LBA, buffer);

    memset(g_Bat, 0, sizeof(g_Bat));

    for (int i = 0; i <= FS_DATA_LBA; i++)
        FS_BatSet(i);

    for (int i = 0; i < g_Superblock.bat_block_count; i++)
        FS_WriteBlock(g_Superblock.bat_start_block + i, g_Bat + (i * 512));

    Directory root = {
        .name = "~",
        .block = g_Superblock.root_dir_block,
        .count = 0,
        .parent = 0xFFFFFFFF
    };
    
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &root, sizeof(Directory));
    FS_WriteBlock(FS_DATA_LBA, buffer);

    return g_Superblock.root_dir_block;
}

uint32_t FS_Load() {
    uint8_t buffer[FS_BLOCK_SIZE];

    FS_ReadBlock(FS_SUPERBLOCK_LBA, buffer);
    memcpy(&g_Superblock, buffer, sizeof(Superblock));
    if (g_Superblock.magic != FS_MAGIC)
        return FS_Initialize();

    for (int i = 0; i < g_Superblock.bat_block_count; i++)
        FS_ReadBlock(g_Superblock.bat_start_block + i, g_Bat + (i * 512));
    
    printf("FS: filesystem loaded; total blocks: %u\n", g_Superblock.bat_block_count);

    return g_Superblock.root_dir_block;
}

void FS_DirCreate(Directory* parent, const char* name) {
    uint32_t new_block = FS_BatFindFreeBlock();
    if (new_block == 0xFFFFFFFF) {
        printf("FS: disk full\n");
        return;
    }

    Directory dir;
    memset(&dir, 0, sizeof(Directory));
    strncpy(dir.name, name, sizeof(dir.name) - 1);
    dir.count = 0;
    dir.block = new_block;
    dir.parent = parent->block;

    uint8_t buffer[FS_BLOCK_SIZE];
    memcpy(buffer, &dir, sizeof(Directory));
    FS_WriteBlock(new_block, buffer);

    if (parent->count >= FS_MAX_ENTRIES) {
        printf("FS: parent folder full\n");
        return;
    }
    parent->entries[parent->count++] = new_block;

    memcpy(buffer, parent, sizeof(Directory));
    FS_WriteBlock(parent->block, buffer);

    FS_BatSet(new_block);
    FS_SaveBat(new_block);
}

void FS_DirDelete(Directory* parent, const char* entry) {
    uint8_t buffer[FS_BLOCK_SIZE];

    for (int i = 0; i < parent->count; i++) {
        FS_ReadBlock(parent->entries[i], buffer);
        Directory candidate;
        memcpy(&candidate, buffer, sizeof(Directory));
        if (memcmp(candidate.name, entry, strlen(entry) + 1) == 0) {
            if (candidate.count > 0) {
                printf("FS: directory is not empty\n");
                return;
            }
            FS_BatClear(candidate.block);
            FS_SaveBat(candidate.block);
        
            for (uint32_t j = i; j < parent->count - 1; j++)
                parent->entries[j] = parent->entries[j + 1];
            parent->count--;
        
            memset(buffer, 0, sizeof(buffer));
            memcpy(buffer, parent, sizeof(Directory));
            FS_WriteBlock(parent->block, buffer);
            return;
        }   
    }
    printf("FS: directory not found\n");
}

void FS_FileCreate() {
    
}

void FS_FileDelete() {

}

void FS_FileRead() {

}

void FS_FileWrite() {

}
