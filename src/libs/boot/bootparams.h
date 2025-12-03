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
    DISK* disk;
    MemoryInfo Memory;
    FatInfo Fat12;
    uint8_t BootDevice;
} BootParams;
