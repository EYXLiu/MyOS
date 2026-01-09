#include "gpu.h"
#include <stdint.h>
#include <arch/i686/pci.h>
#include <debug.h>
#include <arch/i686/mmio.h>

PCIDevice g_Gpu;

void GPU_Initialize() {
    g_Gpu = i686_PCI_Initialize(0x1AF4);
    if (g_Gpu.vendor_id == 0xFFFF) {
        log_err("GPU", "unable to find GPU");
        return;
    }
    log_debug("GPU", "mmio 0x%x", g_Gpu.base);
    i686_MMIO_Write32(g_Gpu.base + 0x14, (1 << 0) | (1 << 1));
    uint32_t features = i686_MMIO_Read32(g_Gpu.base + 0x00);
    i686_MMIO_Write32(g_Gpu.base + 0x04, features);
    uint32_t status = i686_MMIO_Read32(g_Gpu.base + 0x14);
    i686_MMIO_Write32(g_Gpu.base + 0x14, status | (1 << 3));
    status = i686_MMIO_Read32(g_Gpu.base + 0x14);
    if (!(status & (1 << 3))) {
        log_crit("GPU", "features rejected");
    }
}

void GPU_Test() {
}
