#include "ide.h"
#include "io.h"
#include <stdio.h>

#define IDE_DATA 0x1F0
#define IDE_SECTOR_CNT 0x1F2
#define IDE_LBA_LOW 0x1F3
#define IDE_LBA_MID 0x1F4
#define IDE_LBA_HI 0x1F5
#define IDE_DRIVE 0x1F6
#define IDE_COMMAND 0x1F7

#define IDE_CMD_READ 0x20
#define IDE_CMD_WRITE 0x30

void i686_IDE_WaitBSY() {
    while (i686_inb(IDE_COMMAND) & 0x80);
}
void i686_IDE_WaitDRQ() {
    while (!(i686_inb(IDE_COMMAND) & 0x08));
}

void i686_Delay_400ns() {
    i686_inb(IDE_DATA);
    i686_inb(IDE_DATA);
    i686_inb(IDE_DATA);
    i686_inb(IDE_DATA);
}

void i686_IDE_Read(uint32_t lba, uint8_t* buffer) {
    i686_IDE_WaitBSY();

    i686_outb(IDE_SECTOR_CNT, 1); 
    i686_outb(IDE_LBA_LOW, (uint8_t)(lba & 0xFF));
    i686_outb(IDE_LBA_MID, (uint8_t)((lba >> 8) & 0xFF));
    i686_outb(IDE_LBA_HI, (uint8_t)((lba >> 16) & 0xFF));
    i686_outb(IDE_DRIVE, 0xE0 | ((lba >> 24) & 0x0F)); 
    i686_Delay_400ns();

    i686_outb(IDE_COMMAND, IDE_CMD_READ);

    i686_IDE_WaitDRQ();

    // read 1 block (512 bytes) from data port
    for (int i = 0; i < 256; i++) {
        buffer[i] = i686_inw(IDE_DATA);
    }
}

void i686_IDE_Write(uint32_t lba, const uint8_t* buffer) {
    i686_IDE_WaitBSY();

    i686_outb(IDE_SECTOR_CNT, 1);
    i686_outb(IDE_LBA_LOW, (uint8_t)(lba & 0xFF));
    i686_outb(IDE_LBA_MID, (uint8_t)((lba >> 8) & 0xFF));
    i686_outb(IDE_LBA_HI, (uint8_t)((lba >> 16) & 0xFF));
    i686_outb(IDE_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    i686_Delay_400ns();

    i686_outb(IDE_COMMAND, IDE_CMD_WRITE);

    i686_IDE_WaitDRQ();

    // write 1 block (512 bytes) from data port
    for (int i = 0; i < 256; i++) {
        i686_outw(IDE_DATA, buffer[i]);
    }

    i686_IDE_WaitBSY();
}
