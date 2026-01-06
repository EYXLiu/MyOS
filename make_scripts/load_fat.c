#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FS_MAGIC 0x0660D
#define FS_BLOCK_SIZE 512

#define FS_TOTAL_BLOCKS 32768
#define FS_BAT_BLOCKS 8
#define FS_SUPERBLOCK_LBA 0
#define FS_BAT_LBA 1
#define FS_DATA_LBA 9
#define FS_MAX_ENTRIES 116
#define FS_FILE_MAX_SIZE 500

#define FILE_TYPE_NONE 0
#define FILE_TYPE_FILE 1
#define FILE_TYPE_DIR 2

typedef struct {
    uint32_t magic;
    uint32_t total_blocks;
    uint32_t bat_start_block;
    uint32_t bat_block_count;
    uint32_t root_dir_block;
} __attribute__((packed)) Superblock;

typedef struct {
    uint8_t type;
    char name[32];
    uint32_t first_block;
    uint32_t block;
} __attribute__((packed)) FileEntry;

typedef struct {
    uint32_t next_block;
    uint32_t block;
    uint32_t size;
    char data[FS_FILE_MAX_SIZE];
} __attribute__((packed)) FileHeader;

typedef struct {
    uint8_t type;
    char name[32];
    uint32_t count;
    uint32_t block;
    uint32_t parent;
    uint32_t entries[FS_MAX_ENTRIES];
} __attribute__((packed)) Directory;

void write_block(FILE* f, uint32_t block, void* data) {
    fseek(f, block * FS_BLOCK_SIZE, SEEK_SET);
    fwrite(data, 1, FS_BLOCK_SIZE, f);
}

void read_block(FILE* f, uint32_t block, void* data) {
    fseek(f, block * FS_BLOCK_SIZE, SEEK_SET);
    fread(data, 1, FS_BLOCK_SIZE, f);
}

uint32_t allocate_block(uint8_t* bat) {
    for (uint32_t i = FS_DATA_LBA + 1; i < FS_TOTAL_BLOCKS; i++) {
        if (!(bat[i / 8] & (1 << (i % 8)))) {
            bat[i / 8] |= (1 << (i % 8));
            return i;
        }
    }
    return 0xFFFFFFFF; // no free block
}

int main() {
    FILE* f = fopen("build/hdd.img", "wb+");
    FILE* bf = fopen("bgr.bgra", "rb");
    if (!f || !bf) return 1;

    // --- Superblock ---
    Superblock sb = {0};
    sb.magic = FS_MAGIC;
    sb.total_blocks = FS_TOTAL_BLOCKS;
    sb.bat_start_block = FS_BAT_LBA;
    sb.bat_block_count = FS_BAT_BLOCKS;
    sb.root_dir_block = FS_DATA_LBA;
    write_block(f, FS_SUPERBLOCK_LBA, &sb);

    // --- BAT ---
    uint8_t bat[FS_BAT_BLOCKS * FS_BLOCK_SIZE] = {0};
    for (int i = 0; i <= FS_DATA_LBA; i++)
        bat[i / 8] |= (1 << (i % 8));

    for (int i = 0; i < FS_BAT_BLOCKS; i++)
        write_block(f, FS_BAT_LBA + i, bat + i * FS_BLOCK_SIZE);

    // --- Root Directory ---
    Directory root = {0};
    root.type = 2;
    strcpy(root.name, "~");
    root.block = FS_DATA_LBA;
    root.parent = 0xFFFFFFFF;
    root.count = 0;
    write_block(f, FS_DATA_LBA, &root);

    // --- Add bgr.bgra ---
    fseek(bf, 0, SEEK_END);
    size_t filesize = ftell(bf);
    fseek(bf, 0, SEEK_SET);

    uint32_t first_block = 0xFFFFFFFF;
    uint32_t prev_block = 0xFFFFFFFF;
    size_t remaining = filesize;
    uint8_t buffer[FS_FILE_MAX_SIZE];

    while (remaining > 0) {
        size_t chunk = remaining > FS_FILE_MAX_SIZE ? FS_FILE_MAX_SIZE : remaining;
        fread(buffer, 1, chunk, bf);

        uint32_t block = allocate_block(bat);
        if (block == 0xFFFFFFFF) {
            printf("No more free blocks!\n");
            break;
        }

        FileHeader fh = {0};
        fh.block = block;
        fh.next_block = 0xFFFFFFFF;
        fh.size = chunk;
        memcpy(fh.data, buffer, chunk);

        // Write the new block
        write_block(f, block, &fh);

        // Link previous block
        if (prev_block != 0xFFFFFFFF) {
            FileHeader prev_fh;
            read_block(f, prev_block, &prev_fh);
            prev_fh.next_block = block;
            write_block(f, prev_block, &prev_fh);
        } else {
            first_block = block;
        }

        prev_block = block;
        remaining -= chunk;
    }

    // --- Create FileEntry in root ---
    uint32_t fe_block = allocate_block(bat);
    FileEntry fe = {0};
    fe.type = FILE_TYPE_FILE;
    strcpy(fe.name, "bgr.bin");
    fe.first_block = first_block;
    fe.block = fe_block;

    write_block(f, fe_block, &fe);

    // Add entry to root directory
    root.entries[root.count++] = fe_block;
    write_block(f, FS_DATA_LBA, &root);

    // --- Write BAT back ---
    for (int i = 0; i < FS_BAT_BLOCKS; i++)
        write_block(f, FS_BAT_LBA + i, bat + i * FS_BLOCK_SIZE);

    fclose(bf);
    fclose(f);

    printf("File added successfully!\n");
    return 0;
}
