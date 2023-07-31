# Bootloader Info

## What is a Bootloader?

A bootloader is a script that loads the kernel. It is limited to 512 bytes. Since that is such a small amount of space we have to make two separate bootloaders, one that loads the next one and the next one loads the kernel. This bootloader also runs in 32 bit protected mode. When the computer first starts the first thing it does is look for the bootloader. It does this by looking for the magic number which is 0xAA55h. This number is in the last one or two bytes. Once it finds the magic number it is loaded into memmory at 0x7C00. Then booting begins.

## Stage 1

Doesn't do a whole lot. Loads the FAT file system and stage 2 of bootloader.

## Stage 2

Some fun stuff starts to happen. This stage enables 32 bit protected mode, but we also get support for a video mode, and also loads/shifts control to the kernel (big future plans ;P )