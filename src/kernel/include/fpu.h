#include <include/stdio.h>
#include <include/io.h>
#include <stdarg.h>
#include <stdbool.h>

#define asm __asm__
#define volatile __volatile__

void set_fpu(const uint16_t cw);
void enable_fpu(void);