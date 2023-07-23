#ifndef STRINGS_H_
#define STRINGS_H_

#define size_t long

char *strcpy(char *s1, const char *s2);
char *strncpy(char *s1, const char *s2, size_t n);
int strncmp(const char *s1, const char *s2, size_t n);
int convert(char s[]);
char* itoa(int val);

#endif