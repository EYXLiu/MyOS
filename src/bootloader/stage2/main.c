#include <stdint.h>
#include "stdio.h"
#include "disk.h"
#include "fat.h"
#include "memdefs.h"
#include "memory.h"

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

typedef void (*KernelStart)();

void __attribute__((cdecl)) cstart(uint16_t bootDrive) 
{
    clrscr();
    printf("Hello from C\n");
    
    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive)) {
        printf("MAIN: disk init error\n");
        goto end;
    }

    if (!FAT_Initialize(&disk)) {
        printf("MAIN: fat init error\r\n");
        goto end;
    }
    // load from fat
    char buffer[100];
    uint32_t readFat;
    FAT_File* f_fd = FAT_Open(&disk, "text.txt");
    while ((readFat = FAT_Read(&disk, f_fd, sizeof(buffer), buffer)))
    {
        for (uint32_t i = 0; i < readFat; i++)
        {
            if (buffer[i] == '\n')
                putc('\r');
            putc(buffer[i]);
        }
        printf("\r\n");
    }
    FAT_Close(f_fd);


    // load kernel
    FAT_File* k_fd = FAT_Open(&disk, "/kernel.bin");
    uint32_t readKernel;
    uint8_t* kernelBuffer = Kernel;
    while ((readKernel = FAT_Read(&disk, k_fd, MEMORY_LOAD_SIZE, KernelLoadBuffer))) {
        memcpy(kernelBuffer, KernelLoadBuffer, readKernel);
        kernelBuffer += readKernel;
    }
    FAT_Close(k_fd);
    
    KernelStart kernelStart = (KernelStart)Kernel;
    kernelStart();

end:
    for (;;);
}
