#include "include/io.h"
#include "include/rtc.h"
#include "include/strings.h"

#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

/* Stolen from OSDev
Register  Contents            Range
 0x00      Seconds             0–59
 0x02      Minutes             0–59
 0x04      Hours               0–23 in 24-hour mode,
 0x05                              1–12 in 12-hour mode, highest bit set if pm
 0x06      Weekday             1–7, Sunday = 1
 0x07      Day of Month        1–31
 0x08      Month               1–12
 0x09      Year                0–99
 0x32      Century (maybe)     19–20?
 0x0A      Status Register A
 0x0B      Status Register B
*/

datetime current_datetime;

uint8_t from_CMOS_reg(uint8_t reg) {
    i686_outb(CMOS_ADDR, reg);
    return i686_inb(CMOS_DATA);
}

int status() {
    uint8_t status = from_CMOS_reg(0x0A);
    return (status >> 7) & 1;
}

void rtc_get_datetime_raw(datetime * t) {
    while (status() == 1) {}

    t->s = (int)from_CMOS_reg(0x00);
    t->m = (int)from_CMOS_reg(0x02);
    t->h = (int)from_CMOS_reg(0x04);
    t->weekday = (int)from_CMOS_reg(0x06);
    t->day = (int)from_CMOS_reg(0x07);
    t->month = (int)from_CMOS_reg(0x08);
    t->year = (int)from_CMOS_reg(0x09);

    uint8_t regB = from_CMOS_reg(0x0B);

    if (!(regB & 0x04)) {
        t->s = ((t->s / 16) * 10) + (t->s % 16);
        t->m = ((t->m / 16) * 10) + (t->m % 16);
        t->h = ((t->h & 0x0F) + (((t->h & 0x70) / 16) * 10)) | (t->h & 0x80);
        t->day = ((t->day / 16) * 10) + (t->day % 16);
        t->month = ((t->month / 16) * 10) + (t->month % 16);
        t->year = ((t->year / 16) * 10) + (t->year % 16);
    }

    if (!(regB & 0x02) && (t->h & 0x80)) {
        t->h = ((t->h & 0x7F) + 12) % 24;
    }
}

void rtc_get_datetime() {
    datetime t1, t2;

    rtc_get_datetime_raw(&t1);
    do {
        t2 = t1;
        rtc_get_datetime_raw(&t1);
    } while (t1.s != t2.s || t1.m != t2.m || t1.h != t2.h ||
    t1.day != t2.day || t1.month != t2.month || t1.year != t2.year);

    current_datetime = t1;
}

const char * get_weekday_str(int day) {
    const char * days[] = {"Sun", "Mon", "Tue", "Wed", "Thurs", "Fri", "Sat"};
    if (day < 1 || day > 7) return "???";
    return days[day - 1];
}

static void strcat_padded(char *dst, int num) {
    if (num < 10) {
        strcat(dst, "0");
    }
    strcat(dst, itoa(num));
}

void datetime_to_string(datetime * t, char * out_buffer) {
    strcpy(out_buffer, get_weekday_str(t->weekday));
    strcat(out_buffer, " ");

    strcat_padded(out_buffer, t->h);
    strcat(out_buffer, ":");
    strcat_padded(out_buffer, t->m);
    strcat(out_buffer, ":");
    strcat_padded(out_buffer, t->s);

    strcat(out_buffer, " ");

    strcat_padded(out_buffer, t->month);
    strcat(out_buffer, "/");
    strcat_padded(out_buffer, t->day);
    strcat(out_buffer, "/");

    int full_year = 2000 + t->year;
    strcat(out_buffer, itoa(full_year));
}

void rtc_init() {
    rtc_get_datetime();
}
