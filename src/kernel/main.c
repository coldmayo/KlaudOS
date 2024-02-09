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
    clrscr();
    HAL_Initialize();
    enable_fpu();
    startUp();
    memInit();
    interrupts_install_idt();
    initFS();
    scroll(2);
    newLine(0);
end:
    for (;;);
}
