#include "memdetect.h"
#include "x86.h"
#include "stdio.h"
#include <boot/bootparams.h>

#define MAX_REGIONS 256

MemoryRegion g_MemoryRegions[MAX_REGIONS];
int g_MemRegionCount;

void Memory_Detect(MemoryInfo* memoryInfo) {
    E820MemoryBlock block;
    uint32_t continuation = 0;
    int ret;

    g_MemRegionCount = 0;

    do {
        ret = x86_E820GetNextBlock(&block, &continuation);
        if (ret <= 0) break;

        g_MemoryRegions[g_MemRegionCount].Begin = block.Base;
        g_MemoryRegions[g_MemRegionCount].Length = block.Length;
        g_MemoryRegions[g_MemRegionCount].Type = block.Type;
        g_MemoryRegions[g_MemRegionCount].ACPI = block.ACPI;
        g_MemRegionCount++;

        printf("E820: base=0x%llx length=0x%llx type=0x%x\n",
            block.Base, block.Length, block.Type);

    } while (continuation != 0);

    memoryInfo->RegionCount = g_MemRegionCount;
    memoryInfo->Regions = g_MemoryRegions;
}

