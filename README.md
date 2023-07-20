# KlaudOS

The first Klaud themed Operating system!

I watched Star Wars Episode 9 opening night in theaters and Klaud was the main character that stuck with me. Years later I was introduced to Operating System development and TempleOS which inspired me further. So, I decided I would give back to Klaud by creating this hobby operating system years later. 

## Features

* double stage bootloader
* gdt, interrupt handler
* keyboard input and printed output
* kernel with commands
* built in x86 assembly and C using gcc

## How to use

### Required Packages
* gcc
* make
* nasm
* qemu-system-x86

### Download From Release
* download disk image and sh file from release
* run run.sh

### Download From Source
* run make toolchain
* run make 
* run run.sh

### Commands
* run klaud --help in the operating system kernel to see the available commands