#include "include/disp.h"
#include "include/mem.h"	
#include "include/strings.h"
#include "include/stdio.h"

static char *fb = (char *)0x000B8000;
char curr_x[6];

uint8_t textColorChange(uint8_t newCol) {
    uint8_t def_col = convert(memread(319,322));
    uint8_t def_text = (convert(memread(319,322)))%10+0x0;
    uint8_t def_back = (convert(memread(319,322)))/10;
    def_text = newCol;
    //def_col = (def_back*0x10) + def_text;
    def_col = def_text;
    return def_col;
}

uint8_t backColorChange(uint8_t newCol) {
    uint8_t def_col = convert(memread(319,322));
    uint8_t def_text = (convert(memread(319,322)))%10;
    uint8_t def_back = def_col - def_text;
    def_back = newCol;
    def_col = (def_back*0x10) + 0x7;
    if (def_back == 0x7) {
        def_col = (def_back*0x10) + 0x0;
    }
    return def_col;
}

void reset() {
    memcpy(curr_x,itoa(2),strlen(itoa(100))+3);
}

void move_curs(int x) {
    int c_x = convert(curr_x);
    int xi = c_x%80;
    int y = c_x/80;
    //printf("%d",y);
    setcursor(xi+x,y);
    memcpy(curr_x,itoa(convert(curr_x)+x),8);
}

void scroll(int x) {
    int c_x = convert(curr_x);
    c_x = (c_x + (80*x) - c_x%80)+2;
    if (c_x >= 1843) {
        memcpy(curr_x,itoa(1842+80),8);
    } else {
        memcpy(curr_x,itoa(c_x),8);
    }
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
	int c_x = convert(curr_x);
    if (c == '\n') {
        scroll(1);
    } else if (c == '`') {
        fb[i-2] = '\0';
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

void fb_string(unsigned int i, char * s, unsigned char fg, unsigned char bg) {
    int j = 0;
    while(*s != '\0') {
        fb_write_cell(+j, *s, fg, bg);
        s++;
        j++;
    }
}

void fb_write(char c, unsigned int i){
    uint8_t def_col = convert(memread(319,322));
    uint8_t def_text = (convert(memread(319,322)))%10;
    uint8_t def_back = (convert(memread(319,322)))/10;
    int c_x = convert(curr_x);
	fb_write_cell(c_x*2, c, def_back, def_text);
    int xi = c_x%80;
    int y = c_x/80;
    setcursor(xi+1,y);
}

void fb_clear(unsigned int i){
    uint8_t def_col = convert(memread(319,322));
    uint8_t def_text = (convert(memread(319,322)))%10;
    uint8_t def_back = (convert(memread(319,322)))/10;
    int c_x = convert(curr_x);
    fb_write_cell(c_x*2+1, '`', def_back, def_back);
}