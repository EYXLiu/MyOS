#pragma once

#include <stdint.h>
#include <stdbool.h>

#define FS_MAGIC 0xD0660
#define FS_BLOCK_SIZE 512

#define FS_TOTAL_BLOCKS 32768
#define FS_BAT_BLOCKS 8
#define FS_SUPERBLOCK_LBA 0
#define FS_BAT_LBA 1
#define FS_DATA_LBA 9
#define FS_MAX_ENTRIES 117 // max that fits into a blocksize

typedef struct {
    uint32_t magic;
    uint32_t total_blocks;
    uint32_t bat_start_block;
    uint32_t bat_block_count;
    uint32_t root_dir_block;
} __attribute__((packed)) Superblock;

typedef struct {
    char name[32];
    uint32_t size;
    uint32_t first_block;
    uint8_t type;
} __attribute__((packed)) FileEntry;

typedef struct {
    uint32_t next_block;
} __attribute__((packed)) BlockHeader;

typedef struct {
    char name[32];
    uint32_t count;
    uint32_t block;
    uint32_t parent;
    uint32_t entries[FS_MAX_ENTRIES];
} __attribute__((packed)) Directory;

void FS_ReadBlock(uint32_t block, void* buffer);
void FS_WriteBlock(uint32_t block, const void* buffer);
void FS_BatSet(uint32_t block);
void FS_BatClear(uint32_t block);
bool FS_BatIsSet(uint32_t block);
int FS_BatFindFreeBlock();
void FS_SaveBat(uint32_t block);
void FS_SetDirectory(Directory* emptyDir, uint32_t block);

uint32_t FS_Initialize();
uint32_t FS_Load();

void FS_DirCreate(Directory* parent, const char* name);
void FS_DirDelete(Directory* parent, uint32_t index);

void FS_FileCreate();
void FS_FileDelete();
void FS_FileRead();
void FS_FileWrite();

void FS_LS(Directory* dir);
void FS_CD(Directory* dir, const char* entry);
