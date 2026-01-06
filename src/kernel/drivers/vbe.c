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

    log_debug("VBE", "framebuffer initialized to %x", modeInfo->framebuffer);
}

void VBE_SetBG(Directory* parent, const char* s) {
    uint32_t* fb = (uint32_t*)modeInfo->framebuffer;
    uint16_t w = modeInfo->width;
    uint16_t h = modeInfo->height;
    uint16_t pitch = modeInfo->pitch;
    log_debug("VBE", "%u", pitch * h);
    FS_FileRead(parent, s, fb, pitch * h);
}
