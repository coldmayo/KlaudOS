#include "include/disp.h"
#include "include/mem.h"
#include "include/strings.h"
#include "include/stdio.h"
#define PROMPT_OFFSET 0

static char *fb = (char *)0x000B8000;

// Track the cursor natively as an integer to prevent string-parsing bugs
static int current_cursor_pos = 2;

struct template {
    uint8_t temp; // Full attribute byte (background << 4 | foreground)
    uint8_t back;
    uint8_t text;
};

// Initialize with a safe default: Light Gray (7) on Black (0) = 0x07
struct template TMP = { .temp = 0x07, .back = 0x00, .text = 0x07 };

void reset() {
    current_cursor_pos = PROMPT_OFFSET; // Initial horizontal offset matching your original design
    setcursor(PROMPT_OFFSET, 0);
}

uint8_t textColorChange(uint8_t newCol, uint8_t old) {
    clrscr();
    TMP.text = newCol;
    TMP.back = old - (old % 10);
    TMP.temp = (TMP.back) + TMP.text;
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            putchr(x, y, ' ');
            putcolor(x, y, TMP.temp);
        }
    }
    reset();
    return TMP.temp;
}

uint8_t backColorChange(uint8_t newCol, uint8_t old) {
    clrscr();
    TMP.back = newCol;
    TMP.text = old % 10;
    TMP.temp = (TMP.back * 0x10) + TMP.text;
    for (int y = 0; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            putchr(x, y, ' ');
            putcolor(x, y, TMP.temp);
        }
    }
    reset();
    return TMP.temp;
}

void move_curs(int x) {
    current_cursor_pos += x;
    if (current_cursor_pos < 0) current_cursor_pos = 0;
    if (current_cursor_pos >= 80 * 25) current_cursor_pos = (80 * 25) - 1;

    int xi = current_cursor_pos % 80;
    int y = current_cursor_pos / 80;
    setcursor(xi, y);
}

void scroll(int x) {
    // Jump down by line blocks
    current_cursor_pos = (current_cursor_pos + (80 * x) - (current_cursor_pos % 80));
    if (current_cursor_pos >= 80 * 25) {
        current_cursor_pos = (80 * 25) - 80; // Clamp to the final terminal row
    }
    move_curs(0); // Synchronize hardware cursor register
}

void fb_write_cell(unsigned int video_memory_index, char c, uint8_t color) {
    if (c == '\n') {
        scroll(1);
    } else if (c == '\b') {
        if (current_cursor_pos > PROMPT_OFFSET) { // Guard your prompt offset
            current_cursor_pos--;
            int offset = current_cursor_pos * 2;
            fb[offset] = ' ';
            fb[offset + 1] = color;
        }
    } else {
        fb[video_memory_index] = color;
        fb[video_memory_index] = c;
        current_cursor_pos++;
    }
}

void fb_write(char c, unsigned int i) {
    // Use TMP.temp attribute byte so text is visible (not uninitialized black-on-black)
    fb_write_cell(current_cursor_pos * 2, c, TMP.temp);

    int xi = current_cursor_pos % 80;
    int y = current_cursor_pos / 80;
    setcursor(xi, y);
}

void fb_clear(unsigned int i) {
    // Send a backspace down to trigger backspace cell removal logic cleanly
    fb_write_cell(current_cursor_pos * 2, '\b', TMP.temp);

    int xi = current_cursor_pos % 80;
    int y = current_cursor_pos / 80;
    setcursor(xi, y);
}

void fb_string(unsigned int i, char *s, uint8_t color) {
    while (*s != '\0') {
        fb_write(*s, 0);
        s++;
    }
}
