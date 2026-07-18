#ifndef INCLUDE_INTERRUPTS
#define INCLUDE_INTERRUPTS

#include <stdint.h>

extern volatile int line_ready;
extern char shell_line[256];

struct IDT 
{
	unsigned short size;
	unsigned int address;
} __attribute__((packed));

struct IDTDescriptor {
	/* The lowest 32 bits */
	unsigned short offset_low; // offset bits 0..15
	unsigned short segment_selector; // a code segment selector in GDT or LDT
	
	/* The highest 32 bits */
	unsigned char reserved; // Just 0.
	unsigned char type_and_attr; // type and attributes
	unsigned short offset_high; // offset bits 16..31
} __attribute__((packed));

void interrupts_install_idt();

// Wrappers around ASM.
void load_idt(unsigned int idt_address);
void interrupt_handler_33();
void interrupt_handler_14();
void interrupt_handler_32();
void interrupt_handler_38();

struct cpu_state {
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
	unsigned int ebp; 
	unsigned int esi; 
	unsigned int edi; 
} __attribute__((packed));

struct stack_state {
	unsigned int error_code;
	unsigned int eip;
	unsigned int cs;
	unsigned int eflags;
} __attribute__((packed));

void interrupt_handler(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack);

int * better_time(unsigned int time);

double get_update_seconds();

uint64_t get_ticks();

void user_input(char * input);
#endif /* INCLUDE_INTERRUPTS */
