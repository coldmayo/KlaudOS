#include "disp.h"	

static char *fb = (char *)0x000B8000;
char curr_x[6];

void fb_move_cursor(unsigned short pos) {
	i686_outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
	i686_outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
	i686_outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  	i686_outb(FB_DATA_PORT, pos & 0x00FF);
}

void reset() {
    memcpy(curr_x,itoa(3),strlen(itoa(100))+1);
}

void move_curs(int x) {
    memcpy(curr_x,itoa(convert(curr_x)+x),strlen(itoa(curr_x+x))+1);
}

void scroll(int x) {
    int c_x = convert(curr_x);
    c_x = (c_x + (80*x) - c_x%80)+2;
    //printf("%d",c_x);
    if (c_x >= 1842) {
        //printf("peen");
        memcpy(curr_x,itoa(1842+80),strlen(itoa(c_x))+1);
    } else {
        memcpy(curr_x,itoa(c_x),strlen(itoa(c_x))+1);
    }
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
	int c_x = convert(curr_x);
    if (c == '\n') {
        //c_x = c_x + 80 - c_x%80;
        scroll(1);
        //printf("%d",i);
    } else if (c == '$') {
        fb[i-2] = ' ';
	    fb[i] = ((fg & 0x0F) << 4) | (bg & 0x0F);
        c_x--;
        memcpy(curr_x,itoa(c_x),strlen(itoa(c_x))+1);
    } else {
        fb[i] = c;
	    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
        c_x++;
        memcpy(curr_x,itoa(c_x),strlen(itoa(c_x))+1);
    }
}

void fb_write(char c, unsigned int i){
    int c_x = convert(curr_x);
    //printf("%d",c_x);
	fb_write_cell(c_x*2, c, BLACK, LIGHT_GREY);
	fb_move_cursor(c_x+1);
}

void fb_clear(unsigned int i){
    int c_x = convert(curr_x);
    fb_write_cell(c_x*2+1, '$', BLACK, BLACK);
}