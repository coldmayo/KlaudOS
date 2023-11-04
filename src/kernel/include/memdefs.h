#pragma once

// 0x00000000 - 0x000003FF - interrupt vector table
// 0x00000400 - 0x000004FF - BIOS data area

#define MEMORY_MIN          0x00000500
#define MEMORY_MAX          0x00080000

// 0x00000500 - 0x00010500 - FAT driver
#define MEMORY_FAT_ADDR     ((void*)0x20000)
#define MEMORY_FAT_SIZE     0x00010000

#define MEMORY_LOAD_KERNEL  ((void*)0x30000)
#define MEMORY_LOAD_SIZE    0x00010000