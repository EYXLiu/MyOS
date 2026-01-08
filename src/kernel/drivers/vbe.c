#include "vbe.h"
#include <kfat/fat.h>
#include <debug.h>
#include <arch/i686/page.h>

#define desiredWidth 1024
#define desiredHeight 768
#define desiredBpp 32
uint16_t pickedMode = 0xFFFF;

#define FB_VIRT 0xE0000000

// initialize graphics
VbeInfoBlock* info;
VbeModeInfo* modeInfo;

void VBE_Initialize(VbeInfoBlock* vbeInfo, VbeModeInfo* vbeMode) {
    info = vbeInfo;
    modeInfo = vbeMode;

    uint32_t fb = modeInfo->framebuffer;
    uint16_t w = modeInfo->width;
    uint16_t h = modeInfo->height;
    uint16_t pitch = modeInfo->pitch;
    uint32_t fb_size = pitch * h;

    uint32_t fb_start = fb & 0xFFFFF000;
    uint32_t fb_end = (fb + fb_size + 0xFFF) & ~0xFFF;  
    uint32_t num_pages = (fb_end - fb_start) / 0x1000;

    for (uint32_t i = 0; i < num_pages; i++) {
        i686_Page_MMIO(
            FB_VIRT + i * 0x1000,
            fb_start + i * 0x1000,
            PAGE_PRESENT | PAGE_RW
        );
    }

    log_debug("VBE", "framebuffer initialized to %x, (phys %x)", FB_VIRT, modeInfo->framebuffer);
}

void VBE_SetBG(Directory* parent, const char* s) {
    uint32_t* fb = (uint32_t*)FB_VIRT;
    uint16_t w = modeInfo->width;
    uint16_t h = modeInfo->height;
    uint16_t pitch = modeInfo->pitch;
    FS_FileRead(parent, s, fb, pitch * h);
}

void VBE_PutP(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= modeInfo->width || y >= modeInfo->height) 
        return;
    
    uint8_t* fb = (uint8_t*)modeInfo->framebuffer;
    uint32_t offset = y * modeInfo->pitch + x * 4;
    *(uint32_t*)(fb + offset) = color;
}

uint32_t BGRA(uint8_t r, uint8_t g, uint8_t b) {
    return (0xFF << 24) | (r << 16) | (g << 8) | b; 
}
