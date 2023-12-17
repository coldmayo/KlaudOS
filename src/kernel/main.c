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
#include "include/vfs.h"

extern uint8_t __bss_start;
extern uint8_t __end;

// inital kernel boot function, welcomes user to greatest experience of their life

void __attribute__((section(".entry"))) start(uint16_t bootDrive) {    
    clrscr();
    HAL_Initialize();
    enable_fpu();
    startUp();
    // TODO: print out files in boot directory in kernel (works in bootloader)
    memInit();
    interrupts_install_idt();
    initFS();
    //printf(findDirName(getCurrDir())); // Prove I actually got the fs working
    scroll(2);
    newLine(0);
end:
    for (;;);
}
