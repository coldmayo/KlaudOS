#include "include/stdio.h"
#include "include/strings.h"
#include "include/math.h"

// did i spell haiku wrong? yes. will i change it? no

// making this felt like slamming my head against a brick wall repeatedly

// I'll fix this bullshit later

void makeLine(int numSyl,int * struc) {
    char noun[30][30] = {"klaud", "mechanic", "trodatome","star wars","booty","troglodyte","cat","automobile","america","subterranean","teleportation","Venezuela","sky","virgin","Dominican Republic","seven syllables typed here","communism and femboys","Saudi Arabia","Avtomat Kalashnikova 1947","Czechoslovakia","Hippopotamus"};
    int nsyl[] = {1,3,3,2,2,3,1,4,4,5,5,4,1,2,7,7,7,6,6,6,5};
    char verb[30][30] = {"is","loves","talks about","begs for","pees on","sees","celebrates","imagines","exchanges"};
    int vsyl[] = {1,1,2,2,2,1,3,3,3};

    // makes syllable structure for sentience 

    int syl = numSyl;
    int i;
    int numLet = 0;
    for (i=0;syl!=0;i++) {
        int newWord = abs(randint(syl,1));
        if (newWord != 0) {
            syl = syl-newWord;
            if (syl >= 0 && newWord > 0) {
                struc[i] = newWord;
                //printf("%d",newWord);
                numLet++;
            } else {
                syl += newWord;
            }
        }
    }
    printf("\n");

    // using the created structure we can make the sentience
    // for 3+ words: noun, verb, noun, ...
    // for 2 >= words: noun, noun, ...

    int j=0;
    for (j=0;j<numLet;j++) {
        if (i>=3 && i%2 != 0) {
            int k;
            int ops[3];
            int l=0;
            for (k=0;k<=20;k++) {
                if (j%2 == 0) {
                    if (nsyl[k] == struc[j]) {
                        ops[l] = k;
                        l++;
                    }
                } else {
                    if (vsyl[k] == struc[j]) {
                        ops[l] = k;
                        l++;
                    }
                }
            }
            //printf("\n");
            int randNum = abs(randint(2,0));
            //printf("%d",randNum);
            if (j%2==0) {printf("%s ",noun[ops[randNum]]);}
            else {printf("%s ",verb[ops[randNum]]);}
        } else if (i>=3 && i%2 == 0) {
            int k;
            int ops[3];
            int l=0;
            for (k=0;k<=20;k++) {
                if (j%2 == 0 || j==i-1) {
                    if (nsyl[k] == struc[j]) {
                        ops[l] = k;
                        l++;
                    }
                } else {
                    if (vsyl[k] == struc[j]) {
                        //printf("%d %d ",j,i);
                        ops[l] = k;
                        l++;
                    }
                }
            }
            //printf("\n");
            int randNum = abs(randint(2,0));
            //printf("%d",randNum);
            if (j%2==0 || j==i-1) {printf("%s ",noun[ops[randNum]]);}
            else {printf("%s ",verb[ops[randNum]]);}
        } else if (i <= 2) {
            int k;
            int ops[3];
            int l=0;
            for (k=0;k<=20;k++) {
                if (nsyl[k] == struc[j]) {
                    ops[l] = k;
                    //printf("%d",k);
                    l++;
                }
            }
            int randNum = abs(randint(2,0));
            //printf("%d",randNum);
            printf("%s ",noun[ops[randNum]]);
        }
    }
}

void haikuu(void) {
    int struc[7];
    makeLine(5,struc);
    makeLine(7,struc);
    makeLine(5,struc);
}