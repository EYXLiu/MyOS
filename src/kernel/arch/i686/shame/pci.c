#include "pci.h"
#include <stdio.h>
#include <debug.h>
#include "page.h"

PCIDriver g_PCIDriver;
/*
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#define VIRTIO_VENDOR 0x1AF4
#define VIRTIO_BLOCK_DEVICE 0x1001

uint32_t i686_PCI_Config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    return (1U << 31) | ((uint32_t)bus << 16) | ((uint32_t)slot << 11) | ((uint32_t)func << 8) | (offset & 0xFC);
}

void i686_PCI_Initialize() {
    if (!i686_PCI_Find_Virtio_Block(&g_PCIDriver)) {
        printf("VirtIO block device not found\n");
        return;
    }

    printf("VirtIO found at 0x%x\n", g_PCIDriver.bar1);
}

bool i686_PCI_Find_Virtio_Block(PCIDriver* out_dev) {
    for (uint8_t bus = 0; bus < 255; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint16_t vendor = i686_PCI_Read16(bus, slot, 0, 0);
            uint16_t device = i686_PCI_Read16(bus, slot, 0, 2);
            uint16_t bar1 = i686_PCI_Read32(bus, slot, 0, 0x14) & ~0xF;


            if (vendor == 0xFFFF) continue;

            log_debug("PCI", "Bus %u Slot %u Vendor=0x%x Device=0x%x, bar1=0x%x", bus, slot, vendor, device, bar1);
            if (vendor == VIRTIO_VENDOR) {
                out_dev->bus = bus;
                out_dev->slot = slot;
                out_dev->func = 0;
                out_dev->vendor_id = vendor;
                out_dev->device_id = device;
                out_dev->bar1 = i686_PCI_Read32(bus, slot, 0, 0x14) & ~0xF;
                i686_Page_MMIO(out_dev->bar1, out_dev->bar1, PAGE_RW);
                return true;
            }
        }
    }
    return false;
}


uint32_t i686_PCI_Read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    i686_outl(PCI_CONFIG_ADDRESS, i686_PCI_Config(bus, slot, func, offset));
    return i686_inl(PCI_CONFIG_DATA);
}

uint16_t i686_PCI_Read16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t val = i686_PCI_Read32(bus, slot, func, offset);
    return (val >> ((offset & 2) * 8)) & 0xFFFF;
}

uint32_t i686_PCI_Read(uint32_t offset) {
    volatile uint32_t* virtio_base = (volatile uint32_t*)(uintptr_t)g_PCIDriver.bar1;
    return virtio_base[offset / 4];
}

void i686_PCI_Test() {
    
}
*/