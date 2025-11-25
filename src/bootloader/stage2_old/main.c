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

    /*
    FAT_File far* fd_all = FAT_Open(&disk, "/");
    FAT_DirectoryEntry entry;
    while (FAT_ReadEntry(&disk, fd_all, &entry) && *entry.Name != 0x00) {
        if (*entry.Name == 0xE5) continue;
        printf("  ");
        for (int i = 0; i < 11; i++)
            putc(entry.Name[i]);
        printf("\r\n");
    }
    FAT_Close(fd_all);
    */
    

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
        printf("\r\n");
    }
    FAT_Close(fd);

end:
    for (;;);
}
