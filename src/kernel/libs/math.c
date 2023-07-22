#include "math.h"
#include "strings.h"
#include "stdio.h"

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

// function for 4 function calculator
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
            ans = ans-convert(start);
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
        }
    }
    // catches last numbers
    //printf("%d\n",ans);
    if (lastOp == '+') {ans = ans + convert(start);}
    else if (lastOp == '-') {ans = ans - convert(start);}
    else if (lastOp == '*') {ans = ans * convert(start);}
    else if (lastOp == '/') {ans = ans / convert(start);}
    else if (lastOp == '%') {ans = ans % convert(start);}
    else if (lastOp == '^') {ans = pow(ans,convert(start));}
    if (lastOp == '!') {
        if (i > 1) {
            ans = factorial(ans);
        }
    }
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
    int j=0;
    char buffer[256];
    if (k > 9) {
        for (i=0;input[i]!='\0';i++) {
            if (input[i]=='x') {
                int dig1 = k/10;
                int dig2 = k%10;
                buffer[j] = dig1+'0';
                buffer[j+1] = dig2 +'0';
                j++;
            } else {
                buffer[j] = input[i];
            }
            j++;
        }
    } else if (k < 10) {
        for (i=0;i<strlen(input);i++) {
            if (input[i]=='x') {
                buffer[i] = k+'0';
            } else {
                buffer[i] = input[i];
            }
        }
        char buff[strlen(input)+1];
        slice_str(buffer,buffer,0,strlen(input)-1);
    }
    return calc(buffer);
}

// make text based plots

void graph(char *input,int yhi) {
    int x;
    int y;
    for (y=yhi;y>=1;y--) {
        for (x=1;x<=80;x++) {
            int f = eval(input,x);
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