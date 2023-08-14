#include <stdint.h>
#include "include/stdio.h"
#include "include/mem.h"
#include <include/hal.h>
#include "include/keyboard.h"
#include "include/interrupts.h"
#include "include/strings.h"
#include <stddef.h>
#include "include/io.h"
#include "include/sound.h"
#include "include/math.h"
#include "include/disp.h"
#include "include/strings.h"

// user input function. This could rank in my top 10 ugliest pieces of code to date. I'm
// sure as hell not fixing it lmao

// adr 311: rizz
// adr 313: dice
// adr 315: restart

void user_input(char *input) {
    int len = strlen(input);
    char buffer[len + 1];
    static char umoney[6];
    static char kmoney[6];
    uint8_t numLst[8] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8};
    char clrLst[8][12] = {"blue","green","cyan","red","purple","orange","white","grey"};
    if (memread(311,311)[0]=='1') {
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
        memsave(311,"0",1);
        printf("> ");
    } else if (memread(313,313)[0]=='1') {
        if (strcmp(input,"random") == 0) {
            int score = convert(umoney);
            int bscore = convert(kmoney);
            int mroll = randint(6,1) + randint(6,1);
            int kroll = randint(6,1) + randint(6,1);
            int bet;
            if (score > 1) {
                bet = randint(score,1);
            } else {
                printf("'damn u broke ash' he said in his native language\n");
                scroll(1);
                bet = randint(bscore+200,0);
            }
            printf("Bet placed: %d\n",bet);
            if (mroll >= kroll) {
                score = score + bet;
                bscore = bscore - bet;
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
                score = score - bet;
                bscore = bscore + bet;
            }
            memcpy(umoney,itoa(score),6);
            memcpy(kmoney,itoa(bscore),6);
            printf("your roll: %d, klaud roll: %d\n",mroll,kroll);
            printf("your money: %s, klaud money: %s\n",umoney,kmoney);
            scroll(4);
            move_curs(9);
            printf("Type 'exit' to leave the game\nPlace bet>");
            bet = 0;
        } else if (strcmp(input,"exit") != 0) {
            int score = convert(umoney);
            int bscore = convert(kmoney);
            int bet = convert(input);
            int mroll = randint(6,1) + randint(6,1);
            int kroll = randint(6,1) + randint(6,1);
            if (mroll >= kroll) {
                score = score + bet;
                bscore = bscore - bet;
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
                score = score - bet;
                bscore = bscore + bet;
            }
            memcpy(umoney,itoa(score),6);
            memcpy(kmoney,itoa(bscore),6);
            printf("your roll: %d, klaud roll: %d\n",mroll,kroll);
            printf("your money: %s, klaud money: %s\n",umoney,kmoney);
            scroll(3);
            move_curs(9);
            printf("Type 'exit' to leave the game\nPlace bet>");
            bet = 0;
        } else {
            memsave(313,"0",1);
            clrscr();
            printf("> ");
        }
    } else if (memread(315,315)[0]=='1') {
        if (strcmp(input,"yes") == 0 || strcmp(input,"y") == 0) {
            reboot();
        } else {
            scroll(1);
            printf("Restart canceled\n> ");
        }
    } else if (strlen(input) <= 4) {
        printf("Every command starts with klaud, try again");
        scroll(1);
        printf("\n> ");
    } else {
        if (strcmp(slice_str(input,buffer,0,4),"klaud")) {
            printf("Every command starts with klaud, try again");
            printf("\n> ");
            scroll(1);
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
            memsave(311,"1",1);
            clrscr();
            klaud_ascii();
            move_curs(13);
            printf("\nRizz up Klaud>");
        } else if (strcmp(input, "klaud haiku") == 0) {
            haikuu();
            scroll(5);
            printf("\n\n> ");
        } else if (strcmp(input,"klaud ascii") == 0) {
            klaud_ascii();
            printf("\n> ");
        } else if (strcmp(input,"klaud live-slug-reaction") == 0) {
            puts("\n                   ========================================\r\n");
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
            uint8_t numLst[9] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,'\0'};
            char clrLst[9][12] = {"blue","green","cyan","red","purple","orange","white","grey",'\0'};
            int i = 0;
            if (strcmp(slice_str(input,buffer,17,len),"--help")==0) {
                scroll(1);
                printf("klaud text-color supports ");
                // haven't done a while loop in a while wanted to try it out
                // decided to make printing the colors easier in that just adding a color to the clrLst will add it to the colors printed out with the --help command
                i = 0;
                char * col = clrLst[i];
                while (* col != '\0') {
                    if (* clrLst[i+1] == '\0') {
                        printf("and %s",col);
                    } else {
                        printf("%s ",col);
                    }
                    i++;
                    col = clrLst[i];
                }
                printf("\n> ");
            } else if (strcmp(input,"klaud text-color random") == 0) {
                int arrMax = *(&numLst + 1) - numLst - 1;
                int randCol = abs(randint(arrMax-1,0));
                changeColor(numLst[randCol]);
                printf("> Color changed to %s",clrLst[randCol]);
                printf("\n> ");
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
                    } else if (strcmp(slice_str(input,buffer,17,len),"default") == 0) {
                        changeColor(numLst[6]);
                        used++;
                        printf("> Color changed to default");
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
            memsave(315,"1\0",1);
            printf("Are you sure you want to restart?\n> ");
            scroll(1);
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud math") == 0) {
            if (strcmp(slice_str(input,buffer,11,len),"--help")==0) {
                printf("klaud 4 function calculator that supports +,-,/,*,%,^,!\n> ");
                scroll(1);
            } else {
                char * equ = slice_str(input,buffer,11,len);
                scroll(1);
                printf("%d\n> ", calc(equ));
            }
        } else if (strcmp(input,"klaud fun-fact")==0) {
            char factList[12][256] = {
                "Klaud was a mechanic that fought for the Resistance",
                "Klaud belongs to a species called 'Trodatome'",
                "Klaud is 6'2 (1.89 meters) (Klaud could ball???)",
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
            int randNum = abs(randint(arrMax-1,0));
            if (randNum == 9 || randNum == 8) {scroll(2);}
            else {scroll(1);}
            printf("%s\n> ",factList[randNum]);
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud plot")==0) {
            if (strcmp(slice_str(input,buffer,11,15),"point") == 0) {
                if (strcmp(slice_str(input,buffer,11,22),"point random") == 0) {
                    if (isNum(input[24]) == 1) {
                        int iter = convert(slice_str(input,buffer,24,len));
                        int i;
                        for (i = 0;iter > i;i++) {
                            char coords[9] = "\0";
                            int randX = abs(randint(78,0));
                            int randY = abs(randint(20,0));
                            strcat(coords, itoa(randX));
                            strcat(coords, " ");
                            strcat(coords, itoa(randY));
                            plotPoint(coords,22,iter);
                        }
                    } else {
                        char coords[9] = "\0";
                        int randX = abs(randint(78,0));
                        int randY = abs(randint(20,0));
                        strcat(coords, itoa(randX));
                        strcat(coords, " ");
                        strcat(coords, itoa(randY));
                        plotPoint(coords,22,1);
                    }
                    scroll(22);
                    printf("> ");
                } else if (isNum(input[17]) == 1 || input[17] == '(' && isNum(input[18]) == 1) {
                    plotPoint(slice_str(input,buffer,17,len),22,1);
                    scroll(22);
                    printf("> ");
                } else {
                    scroll(1);
                    printf("Use klaud plot --help for proper syntax\n> ");
                }
            } else if (strcmp(slice_str(input,buffer,11,15),"clear") == 0) {
                clearPoints();
                scroll(22);
                printf("points cleared\n> ");
            } else if (strcmp(slice_str(input,buffer,11,16),"--help") == 0) {
                printf("The klaud plotting system.\nExample commands:\nklaud plot x (plots y = x)\nklaud plot point 2 3 (plots point at (2,3))\nklaud plot clear (clears plot)\nNOTE: the max y value for the plots is 21 and 80 for x\n> ");
                scroll(6);
            } else {
                graph(slice_str(input,buffer,11,len),23);
                scroll(23);
                printf("> ");
            }
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud echo")==0) {
            printf("'%s' Klaud said in his native language",slice_str(input,buffer,11,len));
            scroll(1);
            printf("\n> ");
        } else if (strcmp(slice_str(input,buffer,0,9),"klaud dice")==0) {
            memsave(313,"1",1);
            memcpy(umoney,"500",strlen("500")+1);
            memcpy(kmoney,"500",strlen("500")+1);
            clrscr();
            klaud_ascii();
            printf("your money:%s, klaud money: %s",umoney,kmoney);
            move_curs(9);
            printf("\nPlace bet> ");
        } else if (strcmp(slice_str(input,buffer,0,11),"klaud random") == 0) {
            char comm[15][30] = {
                "klaud",
                "klaud ascii",
                "klaud plot x",
                "klaud echo hello world",
                "klaud fun-fact",
                "klaud clear",
                "klaud rizz",
                "klaud dice",
                "klaud text-color ",
                "klaud haiku",
                "klaud --help",
                "klaud live-slug-reaction",
                "klaud shrine",
                "klaud plot point ",
                "klaud memory 0 hello world"
            };
            int arrMax = *(&comm + 1) - comm - 1;
            int clrMax = *(&clrLst + 1) - clrLst - 1;
            int randCom = abs(randint(arrMax,0));
            
            // makes text-color random
            if (strcmp(comm[randCom],"klaud text-color ") == 0 || strcmp(comm[randCom],"klaud text-color") == 0) {
                int randCol = randint(clrMax,0);
                strcat(comm[randCom], clrLst[randCol]);
                printf("%d",randCol);
            // makes plot point random
            } else if (strcmp(comm[randCom],"klaud plot point ") == 0) {
                int randY = randint(19,0);
                int randX = randint(79,0);
                strcat(comm[randCom], itoa(randX));
                strcat(comm[randCom], " ");
                strcat(comm[randCom], itoa(randY));
                printf("%s",comm[randCom]);
            // makes klaud memory
            } else if (strcmp(comm[randCom],"klaud memory 0 hello world") == 0) {
                int randAdr = randint(0,300);
                strcat(comm[randCom], itoa(randAdr));
                strcat(comm[randCom], " ");
                strcat(comm[randCom], "hello world");
            }
            printf("> %s\n",comm[randCom]);
            scroll(1);
            user_input(comm[randCom]);
        } else if (strcmp(slice_str(input,buffer,0,18),"klaud memory --help") == 0) {
            scroll(8);
            printf("The klaud memory system\nklaud memory: edit memory in specific memory location.\nExample: klaud memory 0 hello world\nklaud remember: shows characters at given range of memory locations\nExample: klaud remember 0 5\nklaud free-bytes: see which memory addresses are in use\nklaud del: deletes character at specified address\nExample: klaud del 0 5\n> ");
        } else if (strcmp(slice_str(input,buffer,0,11),"klaud memory") == 0) {
            clrscr();
            int i = 0;
            char * inp = slice_str(input,buffer,13,len);
            char num[4];
            while (isNum(inp[i]) != 0) {
                num[i] = inp[i];
                i++;
            }
            //printf("%s",num);
            int adr = convert(slice_str(num,buffer,0,i-1));
            char *value = slice_str(input,buffer,13+i+1,len);
            //printf("%s",value);
            memsave(adr,value,len);
            scroll(1);
            printf("klaud places value starting from memory address %d\n> ",adr);
        } else if (strcmp(slice_str(input,buffer,0,13),"klaud remember") == 0) {
            clrscr();
            int i = 0;
            char * inp = slice_str(input,buffer,15,len);
            char num[4];
            while (isNum(inp[i]) != 0) {
                num[i] = inp[i];
                i++;
            }
            int adr1 = convert(slice_str(num,buffer,0,i-1));
            int adr2 = convert(slice_str(input,buffer,16+i,len));
            //printf("%d %d",adr1,adr2);
            scroll(1);
            printf("'%s', klaud says in his native language\n> ",memread(adr1,adr2));
        } else if (strcmp(input,"klaud free-bytes") == 0) {
            clrscr();
            memAvail();
            scroll(1);
            printf("\n> ");
        } else if (strcmp(slice_str(input,buffer,0,8),"klaud del") == 0) {
            clrscr();
            int i = 0;
            char * inp = slice_str(input,buffer,11,len);
            char num[4];
            while (inp[i] != ' ') {
                num[i] = inp[i];
                i++;
            }
            int adr1 = convert(num);
            int adr2 = convert(slice_str(input,buffer,12+i,len));
            if (adr1 > adr2) {
                adr2 = adr1;
            }
            i = adr1;
            while (i < adr2+1) {
                memAdrClear(i);
                i++;
            }
            scroll(1);
            printf("%d to %d cleared\n> ",adr1,adr2);
        } else {
            printf("You said: %s, which is not a certified Klaud command. Use the klaud --help command.",input);
            scroll(2);
            printf("\n> ");
        }
    }
}