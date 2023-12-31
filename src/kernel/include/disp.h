#ifndef INCLUDE_FRAMEBUFFER_H
#define INCLUDE_FRAMEBUFFER_H

#pragma once
#include "include/io.h"

#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5

#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

#define BLACK      0
#define BLUE       1
#define GREEN      2
#define CYAN       3
#define RED        4
#define PURPLE     5
#define ORANGE     6
#define WHITE      7
#define GREY       8

#define LOAD_MEMORY_ADDRESS 0xC0000000
#define SCREEN ((uint16_t*)(LOAD_MEMORY_ADDRESS + 0xB8000))
#define WIDTH 80
#define HEIGHT 25
#define DEFAULT_COLOR LIGHT_GREY


/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
void fb_write_cell(unsigned int i, char c, uint8_t color);

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

void scroll(int x);

uint8_t textColorChange(uint8_t newCol);

uint8_t backColorChange(uint8_t newCol);

void move_curs(int x);

void reset(void);

#endif
