#include <stdint.h>
#include "include/stdio.h"
#include "include/memory.h"
#include <include/hal.h>
#include "include/keyboard.h"
#include "include/interrupts.h"
#include "include/strings.h"
#include <stddef.h>
#include "include/io.h"
#include "include/sound.h"
#include "include/math.h"
#include "include/disp.h"

// user input function. This could rank in my top 10 ugliest pieces of code to date. I'm
// sure as hell not fixing it lmao

void user_input(char *input) {
    int len = strlen(input);
    char buffer[len + 1];
    static char rizz[6];
    static char dice[6];
    static char umoney[6];
    static char kmoney[6];
    static char art[6];
    uint8_t numLst[8] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};
    if (strcmp(rizz,"True") == 0) {
        int score = rizzScore(input);
        printf("\n");
        if (score < 0) {
            printf("'Get off my Operating System, I do not consent to this'");
            scroll(3);
        } else if (score == 0) {
            printf("'Whatever you say, I guess.'");
            scroll(2);
        } else if (score > 0) {
            printf("'Finally, someone worth talking to! Some other people I've spoken to have been a huge pain!'");
            scroll(3);
        }
        printf(" he said in his native language\n");
        memcpy(rizz,"False",strlen("False")+1);
        printf("> ");
    } else if (strcmp(dice,"True") == 0) {
        if (strcmp(input,"exit") != 0) {
            int score = convert(umoney);
            int bscore = convert(kmoney);
            int mroll = randint(6,1) + randint(6,1);
            int kroll = randint(6,1) + randint(6,1);
            if (mroll >= kroll) {
                score = score + convert(input);
                bscore = bscore - convert(input);
            } else {
                scroll(1);
                char shitTalk[30][30] = {
                    "couldn't be me tho",
                    "me personally idk",
                    "DAMN",
                    "hell nah",
                    "who's mans is this?"
                };
                printf("'%s' he said in his native language\n",shitTalk[randint(4,0)]);
                score = score - convert(input);
                bscore = bscore + convert(input);
            }
            memcpy(umoney,itoa(score),6);
            memcpy(kmoney,itoa(bscore),6);
            printf("your roll: %d, klaud roll: %d\n",mroll,kroll);
            printf("your money: %s, klaud money: %s\n",umoney,kmoney);
            scroll(3);
            move_curs(9);
            printf("Type 'exit' to leave the game\nPlace bet>");
        } else {
            memcpy(dice,"False",strlen("False")+1);
            clrscr();
            printf("> ");
        }
    } else if (strcmp(art,"True") == 0) {
        clrscr();
        memcpy(art,"False",strlen("False")+1);
        printf("> ");
        
    } else if (strlen(input) <= 4) {
        printf("Every command starts with klaud, try again");
        scroll(1);
        printf("\n> ");
    } else {
        if (strcmp(slice_str(input,buffer,0,4),"klaud")) {
            printf("Every command starts with klaud, try again");
            scroll(1);
            printf("\n> ");
        } else if (strcmp(input, "klaud --help") == 0) {
            help();
            printf("\n> ");
        } else if (strcmp(input, "klaud") == 0) {
            printf("That's the guy. I'm personally a big fan");
            scroll(1);
            printf("\n> ");
        } else if (strcmp(input, "klaud beep") == 0) {
            //beep(950,18);
            printf("will do something soon\n");
            scroll(1);
            printf("> ");
        } else if (strcmp(input, "klaud rizz") == 0) {
            memcpy(rizz,"True",strlen("True")+1);
            clrscr();
            klaud_ascii();
            move_curs(13);
            printf("\nRizz up Klaud>");
        } else if (strcmp(input, "klaud haiku") == 0) {
            printf("Haikuu:\n");
            haikuu();
            scroll(5);
            printf("\n> ");
        } else if (strcmp(input,"klaud ascii") == 0) {
            klaud_ascii();
            printf("\n> ");
        } else if (strcmp(input,"klaud live-slug-reaction") == 0) {
            puts("                   ========================================\r\n");
            klaud_ascii();
            puts("                   ========================================\r\n");
            printf("                      L i v e  S l u g  R e a c t i o n");
            scroll(3);
            printf("\n>");
        } else if (strcmp(input,"klaud shrine") == 0) {
            klaud_ascii();
            flowers();
            printf("\n>");
        // This used to just be a bunch of if statements but now that fixed (thankfully)
        } else if (strcmp(slice_str(input,buffer,0,15),"klaud text-color") == 0) {
            char clrLst[8][12] = {"blue","green","cyan","red","purple","orange","white","grey"};
            int i = 0;
            if (strcmp(slice_str(input,buffer,17,len),"--help")==0) {
                scroll(1);
                printf("klaud text-color supports blue, green, red, purple, orange, white, and grey\n> ");
            } else if (strlen(input) < 19) {
                scroll(1);
                printf("use klaud text-color --help to see available colors\n>");
            } else {
                int used = 0;
                for (int i = 0; i<8; i++) {
                    if (strcmp(slice_str(input,buffer,17,len),clrLst[i]) == 0) {
                        changeColor(numLst[i]);
                        used++;
                        printf("> Color changed to %s",clrLst[i]);
                        printf("\n> ");
                    }
                }
                if (used == 0) {
                    scroll(1);
                    printf("use klaud text-color --help to see available colors\n>");
                }
            }
        } else if (strcmp(input,"klaud clear") == 0) {
            clrscr();
            printf("> ");
        } else if (strcmp(input,"klaud restart") == 0) {
            reboot();
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud math") == 0) {
            if (strcmp(slice_str(input,buffer,11,len),"--help")==0) {
                printf("klaud 4 function calculator that supports +,-,/,*,%,^,!\n> ");
                scroll(1);
            } else {
                char * equ = slice_str(input,buffer,11,len);
                int i;
                scroll(1);
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
                "A Trodatome (species that Klaud is) is a character in Star Wars: Jedi Survivor",
                "Klaud has his own subreddit called r/SaltierThanKlaud (This was just a fun fact please don't use reddit)",
                "According to babycenter.com the name Klaud tripled in popularity since the year Episode 9 came out (2019) in theaters and is still growing",
                "Nick Kellington played Klaud in Episode 9",
                "Chewbacca was the person who convinced Klaud to join the Resistance"
            };
            int arrMax = *(&factList + 1) - factList;
            int randNum = randint(arrMax,0);
            if (randNum == 9 || randNum == 8) {scroll(2);}
            if (randNum == 12) {randNum = 11;}   // im not sure why this works but oh well
            printf("%s\n> ",factList[randNum]);
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud plot")==0) {
            graph(slice_str(input,buffer,11,len),23);
            scroll(23);
            printf("> ");
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud echo")==0) {
            printf("'%s' Klaud said in his native language",slice_str(input,buffer,11,len));
            scroll(1);
            printf("\n> ");
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud dice")==0) {
            memcpy(dice,"True",strlen("True")+1);
            memcpy(umoney,"500",strlen("500")+1);
            memcpy(kmoney,"500",strlen("500")+1);
            clrscr();
            klaud_ascii();
            printf("your money:%s, klaud money: %s",umoney,kmoney);
            move_curs(9);
            printf("\nPlace bet>");
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud art") == 0){
            clrscr();
            memcpy(art,"True",strlen("True")+1);
            printf("Click enter to escape (coming soon)");
        } else {
            printf("You said: %s, which is not a certified Klaud command. Use the klaud --help command.",input);
            scroll(2);
            printf("\n> ");
        }
    }
}