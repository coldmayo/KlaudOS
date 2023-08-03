#include "include/math.h"
#include "include/strings.h"
#include "include/stdio.h"
#include "include/memory.h"

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

// function for 4 function calculator

int calc(char * str) {
    char *start = str;
    int ans = 0;
    char lastOp;
    char opList[7] = {'+','-','/','*','%','^','!'};
    int i=0;   // makes sure the first number doesn't do an operation on the number before that (0)
    for(;*str;str++) { 
        // I hate continuous if/else if statements TODO fix this shit
        if(*str == '+') {
            *str = '\0';
            ans = ans + convert(start);
            if (i == 0) {ans = convert(start);}
            //printf("%d\n%d\n",ans,convert(start));
            *str = '+';
            start = str + 1;
            lastOp = *str;
            i++;
        } else if (*str == '-') {
            *str = '\0';
            ans = ans+(-1*convert(start));
            if (i == 0) {ans = convert(start);}
            *str = '-';
            start = str + 1;
            lastOp = *str;
            i++;
        } else if (*str == '*') {
            *str = '\0';
            ans = convert(start) * ans;
            if (i == 0) {ans = convert(start);}
            *str = '*';
            start = str + 1;
            lastOp = *str;
            i++;
        } else if (*str == '%') {
            *str = '\0';
            ans = ans % convert(start);
            if (i == 0) {ans = convert(start);}
            *str = '%';
            start = str + 1;
            lastOp = *str;
            i++;
        } else if (*str == '/') {
            *str = '\0';
            ans = ans / convert(start);
            if (i == 0) {ans = convert(start);}
            *str = '/';
            start = str + 1;
            lastOp = *str;
            i++;
        } else if (*str == '^') {
            *str = '\0';
            ans = ans * ans;
            ans = pow(ans,convert(start));
            if (i == 0) {ans = convert(start);}
            *str = '^';
            start = str + 1;
            lastOp = *str;
            i++;
        } else if (*str == '!') {
            *str = '\0';
            ans = factorial(convert(start));
            *str = '!';
            start = str + 1;
            lastOp = *str;
            i++;
        } else if (*str == '(' || *str == ')') {
            start = str + 1;
            ans = ans;
        } else if (isNum(*str) == 0) {
                //printf("%s",str);
            printf("'thats not a number dumbass' he said in his native language\n");
            scroll(1);
            return 0;
        }
    }
    // catches last numbers
    if (lastOp == '+') {ans = ans + convert(start);}
    else if (lastOp == '-') {ans = ans - convert(start);}
    else if (lastOp == '*') {ans = ans * convert(start);}
    else if (lastOp == '/') {ans = ans / convert(start);}
    else if (lastOp == '%') {ans = ans % convert(start);}
    else if (lastOp == '^') {ans = pow(ans,convert(start));}
    else if (lastOp == '!') {
        if (i > 1) {
            ans = factorial(ans);
        }
    }
    else {ans = convert(start);}
    return ans;
}

void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

// jesus fucking christ making this function was fucking cancer ill make it more readable soon but I need a break

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
                //printf("%s ",buffer);
            } else if (k < 10) {
                j = i;
                char * num = itoa(k);
                buffer[j] = num[0];
                //printf("%s ",buffer);
            } else if (k > 10) {
                j = i;
                char * num = itoa(k);
                buffer[j] = num[0];
                buffer[j+1] = num[1];
                j++;
                //printf("%s ",buffer);
            }
        } else {
            buffer[j] = input[i];
        }
        j++;
    }
    
    return calc(buffer);
}

// make text based plots

void graph(char *input,int yhi) {
    clrscr();
    int x;
    int y;
    for (y=yhi;y>=1;y--) {
        for (x=1;x<=80;x++) {
            int f = eval(input,x);
            //printf("%d ",f);
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
}

int linReg(int * x, int * y) {
    // linear regression stuff here
}

void plotPoint(char * points, int yhi) {
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
    //printf("%s",numxdum);
    //printf("%s",slice_str(numydum,buffer,0,k-1));
    int f = convert(slice_str(numydum,buffer,0,1)) + 1;
    int x = convert(slice_str(numxdum,buffer,0,1)) + 1;
    //printf("%d %d",x,f);
    
    for (yi=yhi;yi>=1;yi--) {
        for (xi = 1;xi<=80;xi++) {
            if (yi == f && xi == x) {
                printf("o");
            } else if (xi == 1 && yi == 1 && yi != f) {
                printf("+");
            } else if (xi == 1) {
                printf("|");
            } else if (yi == 1) {
                printf("-");
            } else {
                printf(" ");
            }
        }
    }

    // just making sure everything is ACTUALLY set to 0. I have trust issues with this function now
    j = 0;
    i = 0;
    memset(numxdum, '\0', sizeof(numxdum));
    memset(numydum, '\0', sizeof(numydum));
    memset(points, '\0', sizeof(points));
    //printf("%d %d",x,f);
}