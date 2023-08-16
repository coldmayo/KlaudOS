#include "include/mem.h"
#include "include/stdio.h"

char mem[15000];   // main memory

void* memcpy(void* dst, const void* src, uint16_t num) {
    uint8_t* u8Dst = (uint8_t *)dst;
    const uint8_t* u8Src = (const uint8_t *)src;

    for (uint16_t i = 0; i < num; i++)
        u8Dst[i] = u8Src[i];

    return dst;
}

void * memset(void * ptr, int value, uint16_t num) {
    uint8_t* u8Ptr = (uint8_t *)ptr;

    for (uint16_t i = 0; i < num; i++)
        u8Ptr[i] = (uint8_t)value;

    return ptr;
}

int memcmp(const void* ptr1, const void* ptr2, uint16_t num) {
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
        mem[ptr+i] = val[i];
        i++;
    }
}

void memclear () {
    memset(mem,'\0',sizeof(mem));
}

void memAvail() {
    int i;
    int num = 0;
    char * print;
    for (i=0;i<sizeof(mem);i++) {
        if (mem[i] != '\0') {
            strcat(print,", ");
            strcat(print,itoa(i));
            num++;
        }
    }
    if (num == 0) {
        printf("All free");
        scroll(1);
    } else {
        char * buffer;
        print = slice_str(print,buffer,3,strlen(print)+1);   // if i didn't slice it an S would show at the start of the string
        int numScroll = (strlen(print)+1)/80;
        scroll(numScroll+1);
        printf("%s",print);
    }
}

void * memAdrClear (int adr) {
    mem[adr] = '\0';
}

char * memread (int adr1, int adr2) {
    char * buffer;
    char val[256];
    if (adr1 > adr2) {
        adr2 = adr1;
    }
    int i = adr1;
    int j = 0;
    while (i < adr2+1) {
        val[j] = mem[i];
        i++;
        j++;
    }

    return slice_str(val,buffer,0,j);
}