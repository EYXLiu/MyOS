#include "fat.h"
#include <stdio.h>
#include <memory.h>

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
