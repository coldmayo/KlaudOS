#pragma once
#include <stdint.h>
#include <stdarg.h>

void clrscr();

void putc(char c);
void puts(const char* str);
void printf(const char* fmt, ...);
void print_buffer(const char* msg, const void* buffer, uint32_t count);
void user_input(char *input);
int strcmp(char s1[], char s2[]);
unsigned strlen(const char* str);
void append(char s[], char n);
int rand(uint32_t *state);
int randint(int hi, int lo);
char * slice_str(const char * str, char * buffer, int start, int end);
void abort(void);
void changeColor(const uint8_t color);
void klaud_ascii(void);
void flowers(void);
void haikuu(void);
void help(void);
void reboot(void);