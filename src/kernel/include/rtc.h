#ifndef rtc_h_INCLUDED
#define rtc_h_INCLUDED

#include <stdbool.h>

typedef struct{
    int s;
    int m;
    int h;
    int weekday;
    int day;
    int month;
    int year;
    int century;
} datetime;

extern datetime current_datetime;

void rtc_get_datetime();
void datetime_to_string(datetime * t, char * out_buffer);
uint8_t from_CMOS_reg(uint8_t reg);

#endif // rtc_h_INCLUDED
