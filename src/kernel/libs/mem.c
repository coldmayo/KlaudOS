#include "include/mem.h"

char mem[10000];   // main memory

void* memcpy(void* dst, const void* src, uint16_t num)
{
    uint8_t* u8Dst = (uint8_t *)dst;
    const uint8_t* u8Src = (const uint8_t *)src;

    for (uint16_t i = 0; i < num; i++)
        u8Dst[i] = u8Src[i];

    return dst;
}

void * memset(void * ptr, int value, uint16_t num)
{
    uint8_t* u8Ptr = (uint8_t *)ptr;

    for (uint16_t i = 0; i < num; i++)
        u8Ptr[i] = (uint8_t)value;

    return ptr;
}

int memcmp(const void* ptr1, const void* ptr2, uint16_t num)
{
    const uint8_t* u8Ptr1 = (const uint8_t *)ptr1;
    const uint8_t* u8Ptr2 = (const uint8_t *)ptr2;

    for (uint16_t i = 0; i < num; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}

void* memmove(void* dstptr, const void* srcptr, int size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	if (dst < src) {
		for (int i = 0; i < size; i++)
			dst[i] = src[i];
	} else {
		for (int i = size; i != 0; i--)
			dst[i-1] = src[i-1];
	}
	return dstptr;
}

void * memsave (int ptr, char * val, int num) {
    int i = 0;
    while (i < num) {
        if (val[i] != '\0') {
            mem[ptr+i] = val[i];
        } else {
            mem[ptr+i] = ' ';
        }
        i++;
    }
}

void memclear () {
    memset(mem,'\0',sizeof(mem));
}

void memAvail() {
    int i;
    int num = 0;
    for (i=0;i<sizeof(mem);i++) {
        if (mem[i] != '\0') {
            if (num == 0) {
                printf("All but: %d",i);
            } else {
                printf(", %d",i);   
            }
            num++;
        }
    }
    if (num == 0) {
        printf("All free");
    }
}

void * memAdrClear (int adr) {
    mem[adr] = '\0';
}

char memread (int ptr) {
    return mem[ptr];
}