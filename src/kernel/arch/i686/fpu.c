#include "include/fpu.h"
#include <stddef.h>

// set fpu control word

void set_fpu(const uint16_t cw) {
	asm volatile("fldcw %0" :: "m"(cw));
}

//enable fpu (assuming there is one)

void enable_fpu() {
    printf("\nEnabling the FPU\n");
    size_t cr4;
	asm volatile ("mov %%cr4, %0" : "=r"(cr4));
	cr4 |= 0x200;
	asm volatile ("mov %0, %%cr4" :: "r"(cr4));
	set_fpu(0x37F);
}