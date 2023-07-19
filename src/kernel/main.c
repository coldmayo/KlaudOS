#include <stdint.h>
#include "libs/stdio.h"
#include "libs/memory.h"
#include <hal/hal.h>
#include "keyboard.h"
#include "interrupts.h"
#include "libs/strings.h"
#include <stddef.h>
#include "arch/i686/io.h"
#include "sound.h"
#include "libs/math.h"

extern uint8_t __bss_start;
extern uint8_t __end;

// function for checking which words are "bad" and "good" deemed by Klaud. The more good words you have the higher in favor Klaud will have you
int wordCheck(char * str) {
    char bad[40][10] = {
        "ass","penis","piss","cum","boobs","boob","porn","booty","ass","titty","suck","balls","head","nigga","n","word","slur","bitch","slur","klaudussy","kussy","hell","hoe","hoes","astros","texas","boston","poop","poopy","javascript","asshole","butthole","wsg","bbg","shlong"
        };
    char good[40][10] = {
        "love","enjoy","pretty","gorgeous","engineer","arms","good","flowers","trodatome","mechanic","falcon","arch","linux","dog","kiss","hi","hello", "yankees","hi","hello","archlinux", "star", "wars","c","c++","like"
        };
    int i;
    int j = 0;
    for (i=0;i<40;i++) {
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

// inital kernel boot function, welcomes user to greatest experience of their life

void __attribute__((section(".entry"))) start(uint16_t bootDrive)
{
    
    memset(&__bss_start, 0, (&__end) - (&__bss_start));

    HAL_Initialize();
    interrupts_install_idt();
    clrscr();
    klaud_ascii();
    printf("\n                         Version -- 0.0.0: July 2023\n");
    printf("                             Welcome to KlaudOS\n");
    printf("Insert command...> ");
end:
    for (;;);
}

// look at all these if statements. very disgusting 

void user_input(char *input) {
    int len = strlen(input);
    char buffer[len + 1];
    static char rizz[6];
    static char dice[6];
    if (strcmp(rizz,"True") == 0) {
        int score = rizzScore(input);
        printf("\n");
        if (score < 0) {
            printf("'Get off my Operating System, I do not consent to this'");
        } else if (score == 0) {
            printf("'Whatever you say, I guess.'");
        } else if (score > 0) {
            printf("'Finally, someone worth talking to! Some other people I've spoken to have been a huge pain!'");
        }
        printf(" he said in his native language\n");
        memcpy(rizz,"False",strlen("False")+1);
        printf("> ");
    } else if (strlen(input) <= 4) {
        printf("Every command starts with klaud, try again");
        printf("\n> ");
    } else {
        if (strcmp(slice_str(input,buffer,0,4),"klaud")) {
            printf("Every command starts with klaud, try again");
            printf("\n> ");
        } else if (strcmp(input, "klaud --help") == 0) {
            help();
            printf("\n> ");
        } else if (strcmp(input, "klaud") == 0) {
            printf("That's the guy. I'm personally a big fan");
            printf("\n> ");
        } else if (strcmp(input, "klaud beep") == 0) {
            //beep(950,18);
            printf("will do something soon\n");
            printf("> ");
        } else if (strcmp(input, "klaud rizz") == 0) {
            memcpy(rizz,"True",strlen("True")+1);
            clrscr();
            klaud_ascii();
            printf("\nRizz up Klaud>");
        } else if (strcmp(input, "klaud haiku") == 0) {
            printf("Haikuu:\n");
            haikuu();
            printf("\n> ");
        } else if (strcmp(input,"klaud ascii") == 0) {
            klaud_ascii();
            printf("\n> ");
        } else if (strcmp(input,"klaud live-slug-reaction") == 0) {
            puts("                   ========================================\r\n");
            klaud_ascii();
            puts("                   ========================================\r\n");
            printf("                      L i v e  S l u g  R e a c t i o n");
            printf("\n>");
        } else if (strcmp(input,"klaud shrine") == 0) {
            klaud_ascii();
            flowers();
            printf("\n>");
        // This used to just be a bunch of if statements but now that fixed (thankfully)
        } else if (strcmp(slice_str(input,buffer,0,15),"klaud text-color") == 0) {
            char clrLst[8][12] = {"blue","green","cyan","red","purple","orange","white","grey"};
            uint8_t numLst[8] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};
            int i = 0;
            for (int i = 0; i<8; i++) {
                if (strcmp(slice_str(input,buffer,17,len),clrLst[i]) == 0) {
                    changeColor(numLst[i]);
                    printf("> Color changed to %s",clrLst[i]);
                    printf("\n> ");
                }
            }
        } else if (strcmp(input,"klaud clear") == 0) {
            clrscr();
            printf("> ");
        } else if (strcmp(input,"klaud restart") == 0) {
            reboot();
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud math") == 0) {
            if (strcmp(slice_str(input,buffer,11,len),"--help")==0) {
                printf("klaud 4 function calculator that supports +,-,/,*,%,^\n> ");
            } else {
                char * equ = slice_str(input,buffer,11,len);
                int i;
                printf("%d\n> ", calc(equ));
            }
        } else if (strcmp(input,"klaud fun-fact")==0) {
            char factList[12][256] = {
                "Klaud was a mechanic that fought for the Resistance",
                "Klaud belongs to a species called 'Trodatome'",
                "Klaud is 6'2 (Klaud could ball???)",
                "The language Klaud speaks is unknown",
                "Klaud has blue eyes",
                "Klaud is a playable character in Lego Star Wars: The Skywalker Saga", "Klaud was widely believed to be homophobic, but that was disproven recently",
                "A Trodatome (species that Klaud is) is a character iklaud mathn Star Wars: Jedi Survivor",
                "Klaud has his own subreddit called r/SaltierThanKlaud (This was just a fun fact use Lemmy instead of Reddit please + plus the dudes over the sub might have a discord now)",
                "According to babycenter.com the name Klaud tripled in popularity since the year Episode 9 came out (2019) in theaters and is still growing",
                "Nick Kellington played Klaud in Episode 9",
                "Chewbacca was the person who convinced Klaud to join the Resistance"
            };
            int arrMax = *(&factList + 1) - factList;
            int randNum = rand(32)%((arrMax+1)-0) + 0;
            if (randNum == 12) {randNum = 11;}   // im not sure why this works but oh well
            printf("%s\n> ",factList[randNum]);
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud plot")==0) {
            graph(slice_str(input,buffer,11,len),22);
            printf("> ");
        } else {
            printf("You said: %s, which is not a certified Klaud command. Use the klaud --help command.",input);
            printf("\n> ");
        }
    }
}