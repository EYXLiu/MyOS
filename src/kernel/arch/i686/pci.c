#include "pci.h"
#include <arch/i686/io.h>
#include <stdio.h>
#include <debug.h>

PCIDevice i686_PCI_Initialize(uint16_t vendor_id) {
    PCIDevice dev;
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t func = 0; func < 8; func++) {
                uint32_t id = i686_PCI_Read(bus, device, func, 0x00);
                uint16_t vendor = id & 0xFFFF;
                uint16_t device_id = (id >> 16) & 0xFFFF;

                if (vendor == 0xFFFF) continue;
                    
                if (vendor == vendor_id) {
                    uint32_t bar0 = i686_PCI_Read(bus, device, func, 0x10);
                    uint16_t base;
                    if (bar0 & 1) {
                        base = bar0 & 0xFFFFFFFC;
                    } else {
                        base = bar0 & 0xFFFFFFF0;
                    }
                    dev.bus = bus;
                    dev.device = device;
                    dev.func = func;
                    dev.vendor_id = vendor;
                    dev.device_id = device_id;
                    dev.base = base;
                    return dev;
                }
            }
        }
    }
    dev.vendor_id = 0xFFFF;
    return dev;
}

uint32_t i686_PCI_Read(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset) {
    uint32_t addr = (1 << 31) | (bus << 16) | (device << 11) | (func << 8) | (offset & 0xFC);
    i686_outl(0xCF8, addr);
    return i686_inl(0xCFC);
}

void i686_PCI_Write(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset, uint32_t value) {
    uint32_t addr = (1 << 31) | (bus << 16) | (device << 11) | (func << 8) | (offset & 0xFC);
    i686_outl(0xCF8, addr);
    i686_outl(0xCFC, value);
}

uint16_t i686_PCI_Read16(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset) {
    uint32_t value = i686_PCI_Read(bus, device, func, offset);
    return (value >> ((offset & 2) * 8) & 0xFFFF);
}

void i686_PCI_Write16(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset, uint16_t value) {
    uint32_t old = i686_PCI_Read(bus, device, func, offset & 0xFC);
    uint32_t shift = (offset & 2) * 8;
    uint32_t mask = 0xFFFF << shift;
    uint32_t new = (old & ~mask) | (value << shift);
    i686_PCI_Write(bus, device, func, offset & 0xFC, new);
}
