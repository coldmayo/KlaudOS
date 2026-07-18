#pragma once
#include <stdint.h>

void __attribute__((cdecl)) i686_outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) i686_inb(uint16_t port);
void __attribute__((cdecl)) i686_outw(uint16_t port, uint16_t value);
uint8_t __attribute__((cdecl)) i686_inw(uint16_t port);
void __attribute__((cdecl)) i686_outsw(uint16_t port, const void *buffer, uint32_t count);
uint8_t __attribute__((cdecl)) i686_insw(uint16_t port, void *buffer, uint32_t count);

void __attribute__((cdecl)) i686_io_wait(void);
