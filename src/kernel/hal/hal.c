#include "include/hal.h"
#include <include/gdt.h>
#include <include/idt.h>

void HAL_Initialize()
{
    i686_GDT_Initialize();
    i686_IDT_Initialize();
}