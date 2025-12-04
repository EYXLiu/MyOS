#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "io.h"

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t func;
    uint16_t vendor_id;
    uint16_t device_id;
    uint32_t bar1;
} PCIDriver;

void i686_PCI_Initialize();
bool i686_PCI_Find_Virtio_Block(PCIDriver* out_dev);

uint32_t i686_PCI_Read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint16_t i686_PCI_Read16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
uint32_t i686_PCI_Read(uint32_t offset);

void i686_PCI_Test();
