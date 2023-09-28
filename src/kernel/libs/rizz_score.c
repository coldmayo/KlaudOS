#include <stdint.h>
#include "include/stdio.h"
#include <include/hal.h>
#include "include/keyboard.h"
#include "include/interrupts.h"
#include "include/strings.h"
#include <stddef.h>
#include "include/io.h"
#include "include/math.h"
#include "include/disp.h"

// function for checking which words are "bad" and "good" deemed by Klaud. The more good words you have the higher in favor Klaud will have you
int wordCheck(char * str) {
    char bad[60][20] = {
        "ass","penis","piss","cum","boobs","boob","porn","booty","ass","titty","suck","balls","head","nigga","n","word","slur","bitch","slur","klaudussy","kussy","hell","hoe","hoes","astros","texas","boston","poop","poopy","javascript","asshole","butthole","wsg","bbg","shlong","badonkadonk","badonkidonk","fuck","fucking","mommy","daddy","zaddy","papi","windows","microsoft", "shawty"
    };
    char good[60][20] = {
        "love","enjoy","pretty","gorgeous","engineer","arms","good","flowers","trodatome","mechanic","falcon","arch","linux","dog","kiss","hi","hello", "yankees","hi","hello","archlinux", "star", "wars","c","c++","like","archlinux"
    };
    int i;
    int j = 0;
    int arrMax = *(&bad + 1) - bad - 1;
    for (i=0;i<arrMax;i++) {
        //printf("%s ",str);
        if (strcmp(bad[i],str) == 0) {
            j = -1;
        } else if (strcmp(good[i],str) == 0) {
            j = 1;
        }
    }
    return j;
}

// this function converts input (sentences) to an array of all the words in the sentence. It is then
// fed into the function above

int rizzScore(char *str) {
    char *start = str;
    int ans = 0;
    for(;*str;str++) { 
        if(*str == ' ') {
            *str = '\0';
            ans = ans + wordCheck(start);
            *str = ' ';
            start = str + 1;
        }
    }
    ans = ans + wordCheck(start);
    return ans;
}