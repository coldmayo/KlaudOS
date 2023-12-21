#include "include/math.h"
#include "include/strings.h"
#include "include/stdio.h"
#include "include/mem.h"
#include "include/disp.h"

int pointStart = 14000;   // memory address where the points start to be saved

int abs(int i) {
    if (i < 0) {
        i = -1*i;
    }
    return i;
}

int pow(int one, int two) {
    int ans = 1;
    int i;
    for (i=0;i<two;i++) {
        ans = ans * one;
    }
    return ans;
}

int factorial(int x) {
    int ans = 1;
    for (int i=1;i<=x;i++)
        ans *= i;
    return ans;
}

// function for 4 function calculator, very important
// added PEMDAS :D

int calc(char * str) {
    int numTokens[30];
    char opTokens[30];

    // scanner / tokenizer

    int i;
    int j = 0;
    int val = 0;
    for (i=0;strlen(str)+1>i;i++) {
        switch(str[i]) {
            case '+':
                numTokens[j] = val;
                val = 0;
                opTokens[j] = '+';
                j++;
                break;
            case '-':
                numTokens[j] = val;
                val = 0;
                opTokens[j] = '-';
                j++;
                break;
            case '/':
                numTokens[j] = val;
                val = 0;
                opTokens[j] = '/';
                j++;
                break;
            case '*':
                numTokens[j] = val;
                val = 0;
                opTokens[j] = '*';
                j++;
                break;
            case '^':
                numTokens[j] = val;
                val = 0;
                opTokens[j] = '^';
                j++;
                break;
            case '%':
                numTokens[j] = val;
                val = 0;
                opTokens[j] = '%';
                j++;
                break;
            case '!':
                numTokens[j] = val;
                val = 0;
                opTokens[j] = '!';
                j++;
                break;
            default:
                if (isNum(str[i])==1) {
                    val = val*10;
                    int new = str[i] - '0';
                    val += new;
                } else if (isLet(str[i]) == 1) {
                    printf("Error: does %c look like a number to you?\n", str[i]);
                    return -1;
                }
        }
    }

    // add the last number in (if statement in case of factorials)
    numTokens[j] = val;

    // check for exponents

    int ans;
    int arr[20];
    for (i=0;j+1>i;i++) {
        switch (opTokens[i]) {
            case '^':
                ans = pow(numTokens[i],numTokens[i+1]);
                numTokens[i+1] = ans;
                arr[i] = ans;
                break;
            case '!':
                ans = factorial(numTokens[i]);
                numTokens[i] = ans;
                arr[i] = ans;
                break;
            default:
                arr[i] = numTokens[i];
        }
    }

    // check for multiplication/division/modulo

    int arr1[20];
    ans = 0;
    for (i=0;j+1>i;i++) {
        switch (opTokens[i]) {
            case '*':
                ans = arr[i]*arr[i+1];
                arr[i+1] = ans;
                arr1[i] = ans;
                break;
            case '/':
                ans = arr[i]/arr[i+1];
                arr[i+1] = ans;
                arr1[i] = ans;
                break;
            case '%':
                ans = arr[i]%arr[i+1];
                arr[i+1] = ans;
                arr1[i] = ans;
                break;
            default:
                arr1[i] = arr[i];
        }
    }

    // check for addition/subtraction

    int arr2[20];
    ans = 0;
    for (i=0;j+1>i;i++) {
        switch (opTokens[i]) {
            case '+':
                ans = arr1[i]+arr1[i+1];
                arr1[i+1] = ans;
                arr2[i] = ans;
                break;
            case '-':
                ans = arr1[i]-arr1[i+1];
                arr1[i+1] = ans;
                arr2[i] = ans;
                break;
            default:
                arr2[i] = arr1[i];
        }
    }

    return arr2[0];
}

// replaces x character with the number k and then returns the 
// result of that expression from the klaud calculator

int eval(char *input,int k) {
    int i;
    int j = 0;
    char buffer[256];
    for (i=0;i<strlen(input)+2;i++) {
        if (input[i]=='x') {
            if (k == 10) {
                j = i;
                buffer[j] = '1';
                buffer[j+1] = '0';
                j++;
            } else if (k < 10) {
                j = i;
                char * num = itoa(k);
                buffer[j] = num[0];
            } else if (k > 10) {
                j = i;
                char * num = itoa(k);
                buffer[j] = num[0];
                buffer[j+1] = num[1];
                j++;
            }
        } else {
            buffer[j] = input[i];
        }
        j++;
    }
    
    return calc(buffer);
}

// make text based plots from given function

int graph(char *input,int yhi) {
    clrscr();
    int x;
    int y;
    for (y=yhi;y>=1;y--) {
        for (x=1;x<=80;x++) {
            int f = eval(input,x);
            if (f == -1) {
                return -1;
            }
            if (y == f) {
                printf("o");
            } else if (x == 1 && y == 1 && y != f) {
                printf("+");
            } else if (x == 1 && y != f) {
                printf("|");
            } else if (y == 1 && y != f) {
                printf("-");
            } else {
                printf(" ");
            }
        }
    }
    return 0;
}

// clear the points saved

void clearPoints() {
    int i = pointStart;
    int adr2 = 15000;
    while (i < adr2+1) {
        memAdrClear(i);
        i++;
    }
}

// linear regression function 
// math not that hard, literally used notes from 6th grade lmao

int linReg(int * arr) {
    int x[10], y[10];
    int sumX = 0, sumX2=0, sumY=0, sumXY=0, a, b;
    int n = 0;
    int i = 0;
    int j;
    while (arr[n] != 0) {
        y[i] = arr[n] - 1;
        x[i] = arr[n+1] - 1;
        n+=2;
        i+=1;
    }
    for (j=0;j<=i-1;j++) {
        sumX += x[j];
        sumX2 += x[j]*x[j];
        sumY += y[j];
        sumXY += x[j]*y[j];
    }
    j+=1;
    if (j*sumX2-sumX*sumX == 0 || j == 0) {
        printf("Enter another point\n");
    } else {
        b = (j*sumXY-sumX*sumY)/(j*sumX2-sumX*sumX);
        a = (sumY - b*sumX)/j;
        printf("Equation of best fit is: y = %d + %dx\n",a,b);
    }
}

// I had to refactor this once I realized I wanted to plot multiple points
// It's a lot easier if I make a matrix (basically string array) and save points to that
// instead of printing straight away

// sorry for ugly code but if it works it works

void plotPoint(char * points, int yhi, int pltN) {
    clrscr();

    int xi;
    int yi;
    int i = 0;
    int j = 0;
    int k = 0;
    char numxdum[2] = {'\0','\0'};
    char numydum[2] = {'\0','\0'};
    char * thing = numxdum;
    char buffer[2];
    
    // extracting given x and y coordinate for plotting

    while (points[i] != '\0') {
        if (isNum(points[i]) == 1) {
            //printf("%c",points[i]);
            thing[j] = points[i];
            j++;
        } else if (points[i] == '(') {
            j = 0;
        } else if (points[i] == ')') {
            j = 0;
        } else if (isNum(points[i]) != 1) {
            k = j;
            j = 0;
            //printf("%s",thing);
            thing = numydum;
        }
        i++;
    }
    int f = convert(slice_str(numydum,buffer,0,1)) + 1;
    int x = convert(slice_str(numxdum,buffer,0,1)) + 1;

    // plot is saved into a 80 (width of screen by cells) by yhi matrix
    // this is to make it easier to plot more than one point

    char plot[yhi][80];

    for (yi=1;yi<=yhi;yi++) {
        for (xi = 1;xi<=80;xi++) {
            plot[yi][xi] = ' ';
        }
    }
    for(j=1;j<yhi;j++) {
        plot[j][1] = '|';
    }
    for(i=1;i<80;i++) {
        plot[1][i] = '-';
    }
    plot[1][1] = '+';
    
    if (f > 21) {
        f = 21;
    }

    if (x > 80) {
        x = 79;
    }

    // gets saved points and plots them

    int n = 0;
    int o = 0;
    int p = 0;
    int arr[256] = {0,0};
    memset(arr, '\0', sizeof(arr));
    char * allPoints = memread(pointStart,15000);
    char buff[256] = {'\0','\0'};
    while (allPoints[n] != '\0') {
        if (isNum(allPoints[n])==1) {
            buff[o] = allPoints[n];
            o++;
        } else {
            o = 0;
            arr[p] = convert(slice_str(buff,buffer,0,1));
            p++;
            memset(buff, '\0', sizeof(buff));
        }
        n++;
    }

    arr[n+1] = 0;
    n = 0;
    int save = 0;
    while (arr[n] != 0) {
        plot[arr[n]][arr[n+1]] = 'O';
        if (arr[n] == f && arr[n+1] == x) {
            save = 1;
        }
        n+=2;
    }

    if (pltN == 1) {
        plot[f][x] = 'X';
    } else {
        plot[f][x] = 'O';
    }
    arr[n] = f;
    arr[n+1] = x;
    // displays plot

    for (j = yhi; j > 0; j--) {
        for (i = 0; i < 80; i++) {
            putc(plot[j][i]);
        }
    }

    if (pltN == 1) {
        printf("(%d,%d)\n",x-1,f-1);
        linReg(arr);
        scroll(2);
    }

    // saves given point into allPoints if the point isn't already saved
    if (save != 1) {
        strcat(allPoints,itoa(f));
        strcat(allPoints,",");
        strcat(allPoints,itoa(x));
        strcat(allPoints,",");
        memsave(pointStart,allPoints,strlen(allPoints)+1);
    }
}

int firstDigit(int x) {
    while (x > 9) {
        x /= 10;
    }
    return x;
}