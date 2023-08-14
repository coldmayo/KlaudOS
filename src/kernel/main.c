#include <stdint.h>
#include "include/stdio.h"
#include "include/mem.h"
#include <include/hal.h>
#include "include/interrupts.h"
#include "include/strings.h"
#include <stddef.h>
#include "include/io.h"
#include "include/disp.h"

extern uint8_t __bss_start;
extern uint8_t __end;

// inital kernel boot function, welcomes user to greatest experience of their life

void __attribute__((section(".entry"))) start(uint16_t bootDrive) {    
    memset(&__bss_start, 0, (&__end) - (&__bss_start));
    memInit();
    HAL_Initialize();
    clrscr();
    klaud_ascii();
    printf("\n                         Version -- 0.0.5: Aug 2023\n");
    printf("                             Welcome to KlaudOS\n");
    printf("> ");
    scroll(2);
    interrupts_install_idt();
end:
    for (;;);
}