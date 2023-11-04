#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#define min(a,b)    ((a) < (b) ? (a) : (b))
#define max(a,b)    ((a) > (b) ? (a) : (b))

void clrscr();

void putc(char c);
void puts(const char* str);
void printf(const char* fmt, ...);
void setcursor(int x, int y);
void print_buffer(const char* msg, const void* buffer, uint32_t count);
void user_input(char *input);
int strcmp(char s1[], char s2[]);
unsigned strlen(const char* str);
void append(char s[], char n);
int rand(int seed);
int randint(int hi, int lo);
char * slice_str(const char * str, char * buffer, int start, int end);
void abort(void);
void changeColor(const uint8_t color);
void klaud_ascii(void);
void flowers(void);
void haikuu(void);
void help(void);
void startUp(void);
void reboot(void);
void user_input(char *input);
int wordCheck(char * str);
int rizzScore(char *str);
int isNum(char num);
char * lower(char * str);
void memInit(void);
char * upper(char * str);
bool islower(char chr);
bool isupper(char chr);