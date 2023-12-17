#include <stdint.h>
#include "include/stdio.h"
#include "include/x86.h"
#include "include/disk.h"
#include "include/fat.h"
#include "include/memdefs.h"
#include "include/memory.h"
#include "include/vbe.h"

uint8_t* KernelLoadBuffer = (uint8_t*)MEMORY_LOAD_KERNEL;
uint8_t* Kernel = (uint8_t*)MEMORY_KERNEL_ADDR;

typedef void (*KernelStart)();

#define COLOR(r,g,b) ((b) | (g << 8) | (r << 16))

void __attribute__((cdecl)) start(uint16_t bootDrive)
{
    clrscr();

    DISK disk;
    if (!DISK_Initialize(&disk, bootDrive))
    {
        printf("Disk init error\r\n");
        goto end;
    } else {
        printf("Disk initalized");
    }

    if (!FAT_Initialize(&disk))
    {
        printf("FAT init error\r\n");
        goto end;
    } else {
        printf("");
    }
    // browse files in root
    //FAT_File* fd = FAT_Open(&disk, "/");
    //FAT_DirectoryEntry entry;
    //int i = 0;
    //while (FAT_ReadEntry(&disk, fd, &entry) && i++ < 5) {
        //printf("  ");
        //for (int i = 0; i < 11; i++)
            //putc(entry.Name[i]);
        //printf("\r\n");
    //}
    //FAT_Close(fd);

    // load kernel
    FAT_File* fd = FAT_Open(&disk, "/");
    fd = FAT_Open(&disk, "/kernel.bin");
    uint32_t read;
    uint8_t* kernelBuffer = Kernel;
    while ((read = FAT_Read(&disk, fd, MEMORY_LOAD_SIZE, KernelLoadBuffer)))
    {
        memcpy(kernelBuffer, KernelLoadBuffer, read);
        kernelBuffer += read;
    }
    FAT_Close(fd);

    const int desiredWidth = 1024;
    const int desiredHeight = 768;
    const int desiredBpp = 32;
    uint16_t pickedMode = 0xffff;

    // Initialize graphics 
    VbeInfoBlock* info = (VbeInfoBlock*)MEMORY_VESA_INFO;
    VbeModeInfo* modeInfo = (VbeModeInfo*)MEMORY_MODE_INFO;
    bool graphOn = false;   // change to true for graphics mode
    if (VBE_GetControllerInfo(info) && graphOn == true) {
        uint16_t* mode = (uint16_t*)(info->VideoModePtr);
        for (int i = 0; mode[i] != 0xFFFF; i++) {
            if (!VBE_GetModeInfo(mode[i], modeInfo)) {
                printf("Can't get mode info %x :(\n", mode[i]);
                continue;
            }
            bool hasFB = (modeInfo->attributes & 0x90) == 0x90;

            if (hasFB && modeInfo->width == desiredWidth && modeInfo->height == desiredHeight && modeInfo->bpp == desiredBpp) {
                pickedMode = mode[i];
                break;
            }
        }

        if (pickedMode != 0xFFFF && VBE_SetMode(pickedMode)) {
            uint32_t* fb = (uint32_t*)(modeInfo->framebuffer);
            int w = modeInfo->width;
            int h = modeInfo->height;
            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    fb[y * modeInfo->pitch / 4 + x] = COLOR(100, 0, 0);  // add color r g b
                }
            }
        }
    }
    else {
        printf("No VBE extensions :(\n");
    }


    // execute kernel
    KernelStart kernelStart = (KernelStart)Kernel;
    kernelStart();

end:
    for (;;);
}
