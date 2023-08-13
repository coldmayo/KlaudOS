#pragma once
#include "stdint.h"

void* memcpy(void* dst, const void* src, uint16_t num);
void* memset(void* ptr, int value, uint16_t num);
int memcmp(const void* ptr1, const void* ptr2, uint16_t num);
void* memmove(void*, const void*, int);
void * memsave (int ptr, char * val, int num);
char * memread (int adr1, int adr2);
void * memAdrClear (int adr);
void memAvail(void);