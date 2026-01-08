#include <stdint.h>
#include "stdio.h"
#include "disk.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"
#include "vbe.h"
#include "memdetect.h"
#include <boot/bootparams.h>

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;
BootParams g_BootParams;

typedef void (*KernelStart)();

#define COLOR(r, g, b) ((b) | (g << 8) | (r << 16))

void __attribute__((cdecl)) cstart(uint16_t bootDrive) {
    clrscr();
    
    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive)) {
        printf("MAIN: disk init error\n");
        goto end;
    }
    g_BootParams.disk = &disk;


    if (!FAT_Initialize(&disk)) {
        printf("MAIN: fat init error\r\n");
        goto end;
    }
    g_BootParams.Fat12 = FAT_GetParams();

    // load memory
    g_BootParams.BootDevice = bootDrive;
    Memory_Detect(&g_BootParams.Memory);

    //goto end;

    // load kernel
    FAT_File* k_fd = FAT_Open(&disk, "/kernel.bin");
    uint32_t readKernel;
    uint8_t* kernelBuffer = Kernel;
    while ((readKernel = FAT_Read(&disk, k_fd, MEMORY_LOAD_SIZE, KernelLoadBuffer))) {
        memcpy(kernelBuffer, KernelLoadBuffer, readKernel);
        kernelBuffer += readKernel;
    }
    FAT_Close(k_fd);

    const int desiredWidth = 1024;
    const int desiredHeight = 768;
    const int desiredBpp = 32;
    uint16_t pickedMode = 0xFFFF;

    // initialize graphics
    VbeInfoBlock* info = (VbeInfoBlock*)MEMORY_VESA_INFO;
    VbeModeInfo* modeInfo = (VbeModeInfo*)MEMORY_VESA_MODE_INFO;
    if (VBE_GetControllerInfo(info)) {
        uint16_t* mode = (uint16_t*)(info->VideoModePtr);
        for (int i = 0; mode[i] != 0xFFFF; i++) {
            if (!VBE_GetModeInfo(mode[i], modeInfo)) {
                printf("MAIN: can't get mode info %x\n", mode[i]);
                continue;
            }
            bool hasFB = (modeInfo->attributes & 0x90) == 0x90;
            if (hasFB && modeInfo->height == desiredHeight && modeInfo->width == desiredWidth && modeInfo->bpp == desiredBpp) {
                pickedMode = mode[i];
                break;
            }
        }
        if (pickedMode != 0xFFFF) {
            if (!VBE_SetMode(pickedMode)) {
                printf("MAIN: failed to set vbe mode\n");
            }
        }
    } else {
        printf("MAIN: failed to initialize vbe\n");
    }

    g_BootParams.vbeInfo = info;
    g_BootParams.vbeMode = modeInfo;

    KernelStart kernelStart = (KernelStart)Kernel;
    kernelStart(&g_BootParams);

end:
    for (;;);
}
