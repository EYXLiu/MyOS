#include <stdint.h>
#include "stdio.h"
#include "disk.h"

void* g_Data = (void*)0x20000;

void __attribute__((cdecl)) cstart(uint16_t bootDrive) 
{
    clrscr();
    printf("Hello from C\n");
    
    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive)) {
        printf("MAIN: disk init error\n");
        goto end;
    }

    DISK_ReadSectors(&disk, 0, 1, g_Data);
    print_buffer("boot sector: ", g_Data, 512);

    
end:
    for (;;);
}
