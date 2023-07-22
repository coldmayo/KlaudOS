#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

#pragma once
#include "arch/i686/io.h"

/* The I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

/* Frame buffer supported color value */
#define BLACK      0
#define BLUE       1
#define LIGHT_GREY 7

// Make a pixel(2 bytes), background is always black, a is a char, b is foreground color
#define LOAD_MEMORY_ADDRESS 0xC0000000
#define SCREEN ((uint16_t*)(LOAD_MEMORY_ADDRESS + 0xB8000))
#define WIDTH 80
#define HEIGHT 25
#define DEFAULT_COLOR LIGHT_GREY
#define PAINT(a,b) (((b & 0xF) << 8) | (a & 0xFF))

// Get pixel
#define PIXEL(x, y) SCREEN[y * 80 + x]

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos);

/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);

/** write:
 *  writes the contents of the buffer buf of length len to the screen
 *
 *  @param c  character that needs to be written to screen
 *  @param i  number of total characters written on the framebuffer
 */
void fb_write(char c, unsigned int i);

/** clear:
 *  clears the previous content of the buffer and move the cursor back
 *
 *  @param i    number of total characters written on the framebuffer
 */
void fb_clear(unsigned int i);

#endif