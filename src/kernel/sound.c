#include "include/serial_port.h"
#include "include/stdio.h"
#include "include/io.h"

static void PlaySound(uint32_t nFrequence) {
    uint32_t x;
    uint8_t y;
    x = 1193180 / nFrequence;
    i686_outb(0x43, 0xb6);
    i686_outb(0x42, (uint8_t)(x));
    i686_outb(0x42, (uint8_t)(x >> 8));
    y = i686_inb(0x61);
    if (y != (y | 3)) {
 	    i686_outb(0x61, y | 3);
    }
}

static void Quiet() {
    uint8_t x = i686_inb(0x61) & 0xFC;
    i686_outb(0x61, x);
}

void beep(unsigned int freq, unsigned int time) {
    if (!freq) freq = 950;
    if (!time) time = 18;
    if (time == 0) {
        printf("beep warning: 'time' played is zero.");
        return;
    }
    PlaySound(freq);
    //stop(time);
    //Quiet();
}