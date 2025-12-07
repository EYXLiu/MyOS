#include "fat.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>

void FS_LS(Directory* dir) {
    uint8_t buffer[FS_BLOCK_SIZE];
    for (int i = 0; i < dir->count; i++) {
        FS_ReadBlock(dir->entries[i], buffer);
        Directory entry;
        memcpy(&entry, buffer, sizeof(Directory));
        printf("%s ", entry.name);
    }
    printf("\b\n");
}

void FS_CD(Directory* dir, const char* entry) {
    uint8_t buffer[FS_BLOCK_SIZE];
    
    if (memcmp("..", entry, 3) == 0) {
        if (dir->parent == 0xFFFFFFFF) {
            printf("FS: folder does not exist\n");
            return;
        }
        FS_ReadBlock(dir->parent, buffer);
        memcpy(dir, buffer, sizeof(Directory));
        return;
    }

    for (int i = 0; i < dir->count; i++) {
        FS_ReadBlock(dir->entries[i], buffer);
        Directory candidate;
        memcpy(&candidate, buffer, sizeof(Directory));
        if (memcmp(candidate.name, entry, strlen(entry) + 1) == 0) {
            memcpy(dir, &candidate, sizeof(Directory));
            return;
        }
    }
    printf("FS: directory not found\n");
}
