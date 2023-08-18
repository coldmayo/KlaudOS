#include "include/io.h"
#include "include/stdio.h"

#define KEYBOARD_DATA_PORT 0x60

unsigned char keyboard_read_scan_code(void)
{
	return i686_inb(KEYBOARD_DATA_PORT);
}

char keyboard_scan_code_to_ascii(unsigned char scan_code)
{
	char ascii[256] = {
		'\0', '\0', '1', '2', '3', '4', '5', '6',		// 0 - 7
		'7', '8', '9', '0', '-', '=', '\0', '\0',		// 8 - 15
		'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',		// 16 - 23
		'o', 'p', '[', ']', '\0', '\0', 'a', 's',	// 24 - 31
		'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',		// 32 - 39
		'\'', '`', '\0', '\\', 'z', 'x', 'c', 'v',	// 40 - 47
		'b', 'n', 'm', ',', '.', '/', '\0', '*',		// 48 - 55
		'\0', ' ', '\0', '\0', '\0', '\0', '\0', '\0',		// 56 - 63
		'\0', '\0', '\0', '\0', '\0', '\0', '\0', '7',		// 64 - 71
		'8', '9', '-', '4', '5', '6', '+', '1',		// 72 - 79
		'2', '3', '0', '.', '%','^','*','!','|','(',')'				// 80 - 83 plus extras
	};

	return ascii[scan_code];
}
// for shift
char keyboard_scan_code_to_ascii_shift(unsigned char scan_code) {
	char ascii[256] = {
		'\0','\0','!','@','#','$','%','^','&',   // 0 - 8
		'*','(',')','_','+','\0','\0',   // 9 - 15
		'Q','W','E','R','T','Y','U','I',   // 16 - 23
		'O', 'P', '{', '}', '\0', '\0', 'A', 'S',   // 24 - 31
		'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',   // 32 - 39
		'"', '`', '\0', '|', 'Z', 'X', 'C', 'V',	// 40 - 47
		'B', 'N', 'M', '<', '>', '?', '\0', '*',   // 48 - 55
	};
	return ascii[scan_code];
}