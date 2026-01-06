#include "rtl8139.h"
#include <arch/i686/pci.h>
#include <arch/i686/io.h>
#include <debug.h>
#include <mem/block_allocator.h>
#include <memory.h>
#include <arch/i686/irq.h>

#define RTL8139_MAC 0x00
#define RTL8139_MAR 0x08
#define RTL8139_REG_RXBUF 0x30
#define RTL8139_RX_RD_PTR 0x38
#define RTL8139_RX_CMD 0x37
#define RTL8139_TX_STATUS0 0x10
#define RTL8139_TX_ADDR0 0x20
#define RTL8139_CMD 0x37
#define RTL8139_IMR 0x3C
#define RTL8139_ISR 0x3E
#define RTL8139_RCR 0x44
#define RTL8139_TCR 0x40
#define RTL8139_CONFIG1 0x52

#define ROK 0x01
#define TOK 0x04

PCIDevice g_Dev;
uint8_t* rx_buffer;
uint32_t rx_buffer_phys;
static uint32_t rx_offset = 0;
uint8_t mac[6];

void rtl8139_handler(uint8_t isr, uint64_t error, uint64_t irq) {
    uint16_t status = i686_inw(g_Dev.base + RTL8139_ISR);
    i686_outw(g_Dev.base + RTL8139_ISR, 0x35);
    if (status & TOK) {

    }
    if (status & ROK) {
        printf("received\n");
    }
}

void RTL8139_PowerOn() {
    i686_outb(g_Dev.base + RTL8139_CONFIG1, 0x0);
}

void RTL8139_InitRX() {
    i686_outl(g_Dev.base + RTL8139_REG_RXBUF, (uintptr_t)rx_buffer);
}

void RTL8139_SetImrIsr() {
    i686_outw(g_Dev.base + RTL8139_IMR, 0x0005);
}

void RTL8139_ConfigRCR() {
    i686_outl(g_Dev.base + RTL8139_RCR, 0xF | (1 << 7));
}

void RTL8139_InitRXTX() {
    i686_outb(g_Dev.base + RTL8139_RX_CMD, 0x0C);
}

void RTL8139_ReadMac() {
    for (int i = 0; i < 6; i++) {
        mac[i] = i686_inb(g_Dev.base + RTL8139_MAC + i);
    }
}

void RTL8139_Initialize() {
    g_Dev = i686_PCI_Initialize(0x10EC);
    if (g_Dev.vendor_id == 0xFFFF) {
        log_err("RTL8139", "unable to find rtl8139");
        return;
    }
    log_debug("RTL8139", "I/O base: 0x%x", g_Dev.base);

    RTL8139_PowerOn();
    RTL8139_Reset();
    RTL8139_InitRX();
    RTL8139_SetImrIsr();
    RTL8139_ConfigRCR();
    RTL8139_InitRXTX();
    RTL8139_ReadMac();
    log_debug("RTL8139", "MAC address %x:%x:%x:%x:%x:%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void RTL8139_Reset() {
    i686_outb(g_Dev.base + RTL8139_CMD, 0x10);
    while (i686_inb(g_Dev.base + RTL8139_CMD) & 0x10);
}
