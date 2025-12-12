#pragma once

#include <stdint.h>

typedef struct {
    uint8_t bus;
    uint8_t device;
    uint8_t func;
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t base;
} PCIDevice;

PCIDevice i686_PCI_Initialize(uint16_t vendor_id);
uint32_t i686_PCI_Read(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset);
void i686_PCI_Write(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset, uint32_t value);
uint16_t i686_PCI_Read16(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset);
void i686_PCI_Write16(uint8_t bus, uint8_t device, uint8_t func, uint8_t offset, uint16_t value);
