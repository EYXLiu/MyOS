#pragma once

#include <stdint.h>

typedef struct {
    uint8_t id;
    uint16_t cylinders;
    uint16_t sectors;
    uint16_t heads;
} DISK;

typedef struct {
    uint64_t Begin;
    uint64_t Length;
    uint32_t Type;
    uint32_t ACPI;
} MemoryRegion;

typedef struct  {
    int RegionCount;
    MemoryRegion* Regions;
} MemoryInfo;

typedef struct {
    uint32_t FatAddress;
    uint32_t FatSize;
} FatInfo;


typedef struct {
    char VbeSignature[4]; // == VESA
    uint16_t VbeVersion; // 0x0300
    uint16_t OemStringPtr[2]; // vbe far pointer
    uint8_t Capabilities[4];
    uint32_t VideoModePtr; //vbe far pointer
    uint16_t TotalMemory; // # of 64k blocks
    uint8_t _Reserved[236 + 256];
} __attribute__((packed)) VbeInfoBlock;

typedef struct {
    uint16_t attributes;
    uint8_t window_a;
    uint8_t window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a;
    uint16_t segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;
    uint16_t width;
    uint16_t height;
    uint8_t w_char;
    uint8_t y_char;
    uint8_t planes;
    uint8_t bpp;
    uint8_t banks;
    uint8_t memory_model;
    uint8_t bank_size;
    uint8_t image_pages;
    uint8_t reserved0;

    uint8_t red_mask;
    uint8_t red_position;
    uint8_t green_mask;
    uint8_t green_position;
    uint8_t blue_mask;
    uint8_t blue_position;
    uint8_t reserved_mask;
    uint8_t reserved_position;
    uint8_t direct_color_attributes;

    uint32_t framebuffer;
    uint32_t off_screen_mem_off;
    uint32_t off_screen_mem_size;
    uint8_t reserved[206];
} __attribute__((packed)) VbeModeInfo;

typedef struct {
    DISK* disk;
    MemoryInfo Memory;
    FatInfo Fat12;
    uint8_t BootDevice;
    VbeInfoBlock* vbeInfo;
    VbeModeInfo* vbeMode;
} BootParams;
