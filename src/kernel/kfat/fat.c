#include "fat.h"
#include <memory.h>
#include <stdio.h>
#include <arch/i686/ide.h>
#include <string.h>
#include <stdbool.h>

#define FILE_TYPE_NONE 0
#define FILE_TYPE_FILE 1
#define FILE_TYPE_DIR 2

#define FS_BAT_TOTAL (FS_BAT_BLOCKS * FS_BLOCK_SIZE)

static Superblock g_Superblock;
static uint8_t g_Bat[FS_BAT_TOTAL];

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

uint32_t FS_BatFindFreeBlock() {
    for (uint32_t i = 0; i < FS_BAT_TOTAL; i++) {
        if (!FS_BatIsSet(i)) return i;
    }
    return 0xFFFFFFFF;
}

void FS_BatSave(uint32_t block) {
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
        .parent = 0xFFFFFFFF,
        .type = FILE_TYPE_DIR
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

void FS_SaveDirectory(Directory* dir) {
    uint8_t buffer[FS_BLOCK_SIZE];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, dir, sizeof(Directory));
    FS_WriteBlock(dir->block, buffer);
}

bool FS_Exists(Directory* parent, const char* name, uint8_t type) {
    uint8_t buffer[FS_BLOCK_SIZE];
    for (int i = 0; i < parent->count; i++) {
        FS_ReadBlock(parent->entries[i], buffer);
        if (buffer[0] == type) {
            if (type == FILE_TYPE_DIR) {
                Directory entry;
                memcpy(&entry, buffer, sizeof(Directory));
                if (strncmp(entry.name, name, strlen(name) + 1) == 0)
                    return true;
            } else if (type == FILE_TYPE_FILE) {
                FileEntry entry;
                memcpy(&entry, buffer, sizeof(FileEntry));
                if (strncmp(entry.name, name, strlen(name + 1)) == 0) 
                    return true;
            }
        }
    }
    return false;
}

void FS_DirCreate(Directory* parent, const char* name) {
    if (FS_Exists(parent, name, FILE_TYPE_DIR)) {
        printf("FS: directory already exists\n");
        return;
    }

    if (parent->count >= FS_MAX_ENTRIES) {
        printf("FS: directory full\n");
        return;
    }

    uint32_t header = FS_BatFindFreeBlock();
    if (header == 0xFFFFFFFF) {
        printf("FS: disk full\n");
        return;
    }

    Directory dir;
    memset(&dir, 0, sizeof(Directory));
    strncpy(dir.name, name, sizeof(dir.name) - 1);
    dir.type = FILE_TYPE_DIR;
    dir.count = 0;
    dir.block = header;
    dir.parent = parent->block;

    uint8_t buffer[FS_BLOCK_SIZE];
    memcpy(buffer, &dir, sizeof(Directory));
    FS_WriteBlock(header, buffer);

    parent->entries[parent->count++] = header;
    FS_SaveDirectory(parent);

    FS_BatSet(header);
    FS_BatSave(header);
}

void FS_DirDelete(Directory* parent, const char* name) {
    uint8_t buffer[FS_BLOCK_SIZE];

    for (int i = 0; i < parent->count; i++) {
        FS_ReadBlock(parent->entries[i], buffer);
        if (buffer[0] == FILE_TYPE_DIR) {
            Directory candidate;
            memcpy(&candidate, buffer, sizeof(Directory));
            if (memcmp(candidate.name, name, strlen(name) + 1) == 0) {
                if (candidate.count > 0) {
                    printf("FS: directory is not empty\n");
                    return;
                }
                FS_BatClear(candidate.block);
                FS_BatSave(candidate.block);
            
                for (uint32_t j = i; j < parent->count - 1; j++)
                    parent->entries[j] = parent->entries[j + 1];
                parent->count--;
            
                memset(buffer, 0, sizeof(buffer));
                memcpy(buffer, parent, sizeof(Directory));
                FS_WriteBlock(parent->block, buffer);
                return;
            }
        }
    }
    printf("FS: directory not found\n");
}

FileEntry FS_FindFile(Directory* parent, const char* name) {
    FileEntry fe;
    uint8_t buffer[FS_BLOCK_SIZE];
    for (int i = 0; i < parent->count; i++) {
        FS_ReadBlock(parent->entries[i], buffer);
        if (buffer[0] == FILE_TYPE_FILE) {
            memcpy(&fe, buffer, sizeof(FileEntry));
            if (strncmp(fe.name, name, sizeof(name) + 1) == 0) {
                return fe;
            }
        }
    }
    fe.type = FILE_TYPE_NONE;
    fe.first_block = 0xFFFFFFFF;
    return fe;
}


void FS_FileCreate(Directory* parent, const char* name) {
    if (FS_Exists(parent, name, FILE_TYPE_FILE)) {
        printf("FS: file already exists\n");
        return;
    }

    if (parent->count >= FS_MAX_ENTRIES) {
        printf("FS: directory full\n");
        return;
    }

    uint32_t header = FS_BatFindFreeBlock();
    if (header == 0xFFFFFFFF) {
        printf("FS: disk full\n");
        return;
    }

    FileEntry fe;
    memset(&fe, 0, sizeof(FileEntry));
    strncpy(fe.name, name, sizeof(fe.name) - 1);
    fe.type = FILE_TYPE_FILE;
    fe.first_block = 0xFFFFFFFF;
    fe.block = header;

    uint8_t buffer[FS_BLOCK_SIZE];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, &fe, sizeof(FileEntry));
    FS_WriteBlock(header, buffer);

    parent->entries[parent->count++] = header;
    FS_SaveDirectory(parent);

    FS_BatSet(header);
    FS_BatSave(header);
}

void FS_FileBlockDelete(uint32_t block) {
    if (block == 0xFFFFFFFF) return;

    uint8_t buffer[FS_BLOCK_SIZE];
    memset(buffer, 0, sizeof(buffer));
    FS_ReadBlock(block, buffer);
    FileHeader fh;
    memcpy(buffer, &fh, sizeof(FileHeader));
    FS_BatClear(block);
    FS_BatSave(block);
    FS_FileBlockDelete(fh.next_block);
}

void FS_FileDelete(Directory* parent, const char* name) {
    FileEntry fe = FS_FindFile(parent, name);
    if (fe.type == 0) {
        printf("FS: not a file\n");
        return;
    }
    FS_BatClear(fe.block);
    FS_BatSave(fe.block);
    FS_FileBlockDelete(fe.first_block);
}

void FS_FileBlockPrint(uint32_t block) {
    if (block == 0xFFFFFFFF) return;

    uint8_t buffer[FS_BLOCK_SIZE];
    memset(buffer, 0, sizeof(buffer));
    FS_ReadBlock(block, buffer);
    FileHeader fh;
    memcpy(&fh, buffer, sizeof(FileHeader));
    printf("%s", fh.data);
    FS_FileBlockPrint(fh.next_block);
}

void FS_FilePrint(Directory* parent, const char* name) {
    FileEntry fe = FS_FindFile(parent, name);
    if (fe.type == 0) {
        printf("FS: not a file\n");
        return;
    }
    FS_FileBlockPrint(fe.first_block);
}

void FS_FileClear(Directory* parent, const char* name) {
    FileEntry fe = FS_FindFile(parent, name);
    if (fe.type != FILE_TYPE_FILE) {
        printf("FS: not a file\n");
        return;
    }
    FS_FileBlockDelete(fe.first_block);
    fe.first_block = 0xFFFFFFFF;
}

void FS_SaveEntry(FileEntry* fe) {
    uint8_t buffer[FS_BLOCK_SIZE];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, fe, sizeof(FileEntry));
    FS_WriteBlock(fe->block, buffer);
}

void FS_SaveFile(FileHeader* fh) {
    uint8_t buffer[FS_BLOCK_SIZE];
    memset(buffer, 0, sizeof(buffer));
    memcpy(buffer, fh, sizeof(FileHeader));
    FS_WriteBlock(fh->block, buffer);
}

void FS_FileWrite(Directory* parent, const char* name, const void* buffer, size_t bytes) {
    FS_FileClear(parent, name);
    FS_FileAppend(parent, name, buffer, bytes);
}

void FS_FileAppend(Directory* parent, const char* name, const void* buffer, size_t bytes) {
    const uint8_t* p = buffer;

    FileEntry fe = FS_FindFile(parent, name);
    if (fe.type != FILE_TYPE_FILE) {
        printf("FS: not a file\n");
        return;
    }
    FileHeader fh; 
    uint8_t buf[FS_BLOCK_SIZE];
    uint32_t header;

    if (fe.first_block == 0xFFFFFFFF) {
        header = FS_BatFindFreeBlock();
        FS_BatSet(header);
        FS_BatSave(header);
        fh.block = header;
        fh.next_block = 0xFFFFFFFF;
        fh.size = 0;
        memset(fh.data, 0, sizeof(fh.data));

        fe.first_block = header;
        FS_SaveEntry(&fe);
    } else {
        header = fe.first_block;
        while (header != 0xFFFFFFFF) {
            FS_ReadBlock(header, buf);
            memcpy(&fh, buf, sizeof(FileHeader));
            header = fh.next_block;
        }
    }
    
    while (bytes > 0) {
        uint32_t fs = FS_FILE_MAX_SIZE - fh.size;
        if (fs == 0) {
            header = FS_BatFindFreeBlock();
            FS_BatSet(header);
            FS_BatSave(header);

            fh.next_block = header;
            FS_SaveFile(&fh);

            memset(&fh, 0, sizeof(FileHeader));
            fh.block = header;
            fh.next_block = 0xFFFFFFFF;
            fs = FS_FILE_MAX_SIZE;
        }

        uint32_t chunk = bytes > fs ? fs : bytes;
        memcpy(&fh.data[fh.size], p, chunk);

        fh.size += chunk;
        p += chunk;
        bytes -= chunk;

        FS_SaveFile(&fh);
    } 
}
