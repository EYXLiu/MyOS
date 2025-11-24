#include "stdint.h"
#include "stdio.h"
#include "disk.h"
#include "fat.h"

void _cdecl cstart_(uint16_t bootDrive) {
    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive)) {
        printf("MAIN: disk init error \r\n");
        goto end;
    }

    puts("Hello world from C\r\n");
    printf("Hello world from %s\r\n", "C printf");

    if (!FAT_Initialize(&disk)) {
        printf("MAIN: fat init error\r\n");
        goto end;
    }

    char buffer[100];
    uint32_t read;
    FAT_File far* fd = FAT_Open(&disk, "text.txt");
    while ((read = FAT_Read(&disk, fd, sizeof(buffer), buffer)))
    {
        for (uint32_t i = 0; i < read; i++)
        {
            if (buffer[i] == '\n')
                putc('\r');
            putc(buffer[i]);
        }
    }
    FAT_Close(fd);

end:
    for (;;);
}
