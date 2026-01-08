#include "gpu.h"
#include <stdint.h>
#include <arch/i686/pci.h>
#include <debug.h>

PCIDevice g_Gpu;

void GPU_Initialize() {
    g_Gpu = i686_PCI_Initialize(0x1AF4);
    if (g_Gpu.vendor_id == 0xFFFF) {
        log_err("GPU", "unable to find GPU");
        return;
    }
    log_debug("GPU", "d: 0x%x", g_Gpu.base);

    
}
