// keyboard interupts

#include "interrupts.h"
#include "pic.h"
#include "arch/i686/io.h"
#include "arch/i686/idt.h"
#include "libs/stdio.h"
#include "libs/memory.h"
#include "arch/i686/idt.h"
#include "libs/strings.h"
#include "arch/i686/serial_port.h"
#include "keyboard.h"
#include "libs/disp.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256 
#define INTERRUPTS_KEYBOARD 33
#define INTERRUPTS_TIMER 0 
unsigned int BUFFER_COUNT;

struct IDTDescriptor idt_descriptors[INTERRUPTS_DESCRIPTOR_COUNT];
struct IDT idt;

void interrupts_init_descriptor(int index, unsigned int address)
{
	idt_descriptors[index].offset_high = (address >> 16) & 0xFFFF; // offset bits 0..15
	idt_descriptors[index].offset_low = (address & 0xFFFF); // offset bits 16..31

	idt_descriptors[index].segment_selector = 0x08; // The second (code) segment selector in GDT: one segment is 64b.
	idt_descriptors[index].reserved = 0x00; // Reserved.

	/*
	   Bit:     | 31              16 | 15 | 14 13 | 12 | 11     10 9 8   | 7 6 5 | 4 3 2 1 0 |
	   Content: | offset high        | P  | DPL   | S  | D and  GateType | 0 0 0 | reserved
		P	If the handler is present in memory or not (1 = present, 0 = not present). Set to 0 for unused interrupts or for Paging.
		DPL	Descriptor Privilige Level, the privilege level the handler can be called from (0, 1, 2, 3).
		S	Storage Segment. Set to 0 for interrupt gates.
		D	Size of gate, (1 = 32 bits, 0 = 16 bits).
	*/
	idt_descriptors[index].type_and_attr =	(0x01 << 7) |			// P
						(0x00 << 6) | (0x00 << 5) |	// DPL
						0xe;				// 0b1110=0xE 32-bit interrupt gate
}

void interrupts_install_idt()
{
	interrupts_init_descriptor(INTERRUPTS_TIMER,(unsigned int) interrupt_handler_0);
	interrupts_init_descriptor(INTERRUPTS_KEYBOARD, (unsigned int) interrupt_handler_33);

	idt.address = (int) &idt_descriptors;
	idt.size = sizeof(struct IDTDescriptor) * INTERRUPTS_DESCRIPTOR_COUNT;
	i686_IDT_Load((int) &idt);

	pic_remap(PIC_1_OFFSET, PIC_2_OFFSET);
}

/* Interrupt handlers ********************************************************/

void backspace(char s[]) {
    int len = strlen(s);
    s[len-1] = '\0';
}

void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, unsigned int interrupt, __attribute__((unused)) struct stack_state stack)
{
	unsigned char scan_code;
	unsigned char ascii;
	static char key_buffer[256];
	static char prevComm[256];
	int shift;
	switch (interrupt){
		case INTERRUPTS_KEYBOARD:
			scan_code = keyboard_read_scan_code();
			//printf("%d",scan_code);
			if (scan_code == 28) {
				printf("\n");
				fb_write('\n',BUFFER_COUNT);
				user_input(key_buffer);
				memcpy(prevComm,key_buffer,strlen(key_buffer)+1);
				key_buffer[0] = '\0';
			} else if (scan_code == 1) {
				printf("\n> ");
				key_buffer[0] = '\0';
				scroll(1);
			} else if (scan_code == 72) {
				memcpy(key_buffer,prevComm,strlen(key_buffer)+1);
				move_curs(strlen(key_buffer));
				printf("%s",key_buffer);
			} else if (shift == 1) {
				if (scan_code==13) {scan_code = 78;}
				else if (scan_code==6) {scan_code = 84;}
				else if (scan_code==7) {scan_code = 85;}
				else if (scan_code==9) {scan_code = 86;}
				else if (scan_code==2) {scan_code = 87;}
				else if (scan_code==43) {scan_code = 88;}
				ascii = keyboard_scan_code_to_ascii(scan_code);
				append(key_buffer, ascii);
				fb_write(ascii,BUFFER_COUNT);
				//printf("%c",ascii);
				shift = 0;
			} else if (scan_code == 14) {
				backspace(key_buffer);
				BUFFER_COUNT--;
				fb_clear(BUFFER_COUNT);
			} else if (scan_code == 42) {
				shift = 1;
			} else if (scan_code == 77) {
				
			} else if (scan_code == 75) {
				
			} else if (scan_code <= KEYBOARD_MAX_ASCII) {
				ascii = keyboard_scan_code_to_ascii(scan_code);
				append(key_buffer, ascii);
				fb_write(ascii,BUFFER_COUNT);
				//putc(ascii);
				BUFFER_COUNT++;
				//printf("%c",ascii);
			}
			pic_acknowledge(interrupt);

			break;
		case INTERRUPTS_TIMER:
			printf("penis");
			pic_acknowledge(interrupt);
			break;
		default:
			break;
    }
}