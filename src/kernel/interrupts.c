// keyboard interupts

#include "include/interrupts.h"
#include "include/pic.h"
#include "include/io.h"
#include "include/idt.h"
#include "include/stdio.h"
#include "include/memory.h"
#include "include/idt.h"
#include "include/strings.h"
#include "include/serial_port.h"
#include "include/keyboard.h"
#include "include/disp.h"

#define INTERRUPTS_DESCRIPTOR_COUNT 256 
#define INTERRUPTS_KEYBOARD 33
#define INTERRUPTS_TIMER 0 
unsigned int BUFFER_COUNT;
int lim = 0;
int cursPos = 0;

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

void interrupt_handler(__attribute__((unused)) struct cpu_state cpu, unsigned int interrupt, __attribute__((unused)) struct stack_state stack) {
	unsigned char scan_code;
	unsigned char ascii;
	static char key_buffer[256];
	static char prevComm[256];
	//char prevComm[256][256];   make this array of prev commands
	int shift;
	switch (interrupt){
		case INTERRUPTS_KEYBOARD:
			scan_code = keyboard_read_scan_code();
			//printf("%d",scan_code);
			if (scan_code == 28) {
				printf("\n");
				fb_write('\n',BUFFER_COUNT);
				user_input(key_buffer);
				memcpy(prevComm,key_buffer,lim+1);
				// clearing input
				int i;
				memset(key_buffer, '\0', sizeof(key_buffer));
				lim = 0;
				cursPos = 0;
			} else if (scan_code == 1) {
				printf("\n> ");
				// clearing input
				memset(key_buffer, '\0', sizeof(key_buffer));
				scroll(1);
			} else if (scan_code == 72) {
				memcpy(key_buffer,prevComm,256);
				printf("%s",key_buffer);
				move_curs(strlen(key_buffer));
				lim += strlen(key_buffer);
				cursPos += strlen(key_buffer);
			} else if (shift == 1) {
				if (scan_code==13) {scan_code = 78;}
				else if (scan_code==6) {scan_code = 84;}
				else if (scan_code==7) {scan_code = 85;}
				else if (scan_code==9) {scan_code = 86;}
				else if (scan_code==2) {scan_code = 87;}
				else if (scan_code==43) {scan_code = 88;}
				else if (scan_code==10) {scan_code = 89;}
				else if (scan_code==11) {scan_code = 90;}
				ascii = keyboard_scan_code_to_ascii(scan_code);
				key_buffer[cursPos] = ascii;
				fb_write(ascii,BUFFER_COUNT);
				shift = 0;
				lim++;
				cursPos++;
			} else if (scan_code == 14) {
				if (lim > 0) {
					BUFFER_COUNT--;
					fb_clear(BUFFER_COUNT);
					key_buffer[cursPos-1] = '\0';
					lim--;
					cursPos--;
				}
			} else if (scan_code == 42) {
				shift = 1;
			} else if (scan_code == 77) {
				if (lim > cursPos+1) {
					move_curs(1);
					fb_move_cursor(1);
					cursPos++;
				}
			} else if (scan_code == 75) {
				if (0 < cursPos) {
					move_curs(-1);
					fb_move_cursor(-1);
					cursPos--;
				}
			} else if (scan_code <= KEYBOARD_MAX_ASCII) {
				ascii = keyboard_scan_code_to_ascii(scan_code);
				key_buffer[cursPos] = ascii;
				fb_write(ascii,BUFFER_COUNT);
				BUFFER_COUNT++;
				lim++;
				cursPos++;
			}
			pic_acknowledge(interrupt);

			break;
		case INTERRUPTS_TIMER:
			pic_acknowledge(interrupt);
			break;
		default:
			break;
    }
}