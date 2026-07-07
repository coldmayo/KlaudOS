#include <stdint.h>
#include "include/stdio.h"
#include "include/mem.h"
#include <include/hal.h>
#include "include/interrupts.h"
#include "include/strings.h"
#include "include/fpu.h"
#include <stddef.h>
#include "include/io.h"
#include "include/disp.h"
#include "include/fs.h"

extern uint8_t __bss_start;
extern uint8_t __end;

// inital kernel boot function, welcomes user to greatest experience of their life

void __attribute__((section(".entry"))) start(uint16_t bootDrive) {
    // Zero the .bss section. This is not done automatically for a raw
    // flat binary, so every uninitialized global/static variable
    // (interrupt descriptor tables, tick counters, etc.) would otherwise
    // start out as whatever garbage was left in RAM instead of 0.
    uint8_t* bss = &__bss_start;
    uint8_t* bssEnd = &__end;
    while (bss < bssEnd) {
        *bss++ = 0;
    }

    clrscr();
    HAL_Initialize();
    enable_fpu();
    memInit();
    interrupts_install_idt();
    initFS();
    startUp();
    scroll(3);
    newLine(0);
    end:
    for (;;);
}
