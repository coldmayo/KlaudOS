#include "include/stdio.h"
#include "include/disp.h"
#include "include/fs.h"

void klaud_ascii() {
    puts(                                                                               
    "                                        .       ,(       \r\n"             
    "                                     , (,.*/ ,,          \r\n"         
    "                      ,..         .****(,,,&#     ,,(    \r\n"          
    "                         ****, .,,&&&&&**#(##/.***       \r\n"               
    "                                 (,**/#((#(###/..,..../  \r\n"       
    "                     .....,,,***(((,,**#/*/###/          \r\n"                  
    "                             .*((,,,*,,..,*((#           \r\n"   
    "                       .,, /   **/,,*,***((((#*          \r\n"           
    "                               ,*,(./,#(######(          \r\n"                
    "                              **,,.,..*,,,**//(/         \r\n"               
    "                              /*,,,,**,.,,*(#&#(         \r\n"               
    "                             *,*,**#**,,**((#&&#(        \r\n"              
    "                            ,*,*,.....,**/#((//#(        \r\n"               
    );
    scroll(14);
}

void flowers() {
    puts(
    "                                   _                        \r\n"
    "                                 _(_)_                      \r\n"
    "                    @@@@        (_)@(_)    vVVVv    _       \r\n"
    "                   @@()@@  wWWWw  (_) /    (___)  _(_)_     \r\n"
    "                    @@@@   (___)     `|/    /    (_)@(_)    \r\n"
    "                     /       Y        |     |/    /(_)      \r\n"
    "                     |       |/       | /   | /   |/        \r\n"
    "                     |//     |///     |//   |///  |///      \r\n"
    "                   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  \r\n"
    );
    scroll(9);
}

// lowkey dont know what \r does 

void help() {
    puts(
        "Commands:\r\n"
        "klaud: Everything starts with Klaud.\r\n"
        "klaud echo <phrase>: returns whatever you say back\r\n"
        "klaud dice: play street dice with klaud\r\n"
        "klaud memory <mem adr> <value>: write stuff to memory\r\n"
        "klaud rizz: rizz up Klaud and see what he says\r\n"
        "klaud haiku: klaud makes a haiku! it wont be nice, he doesn't speak english\r\n"
        "klaud text-color <color>: changes the color of kernel text.\r\n"
        "klaud back-color <color>: changes the color of the kernel background\r\n"
        "klaud clear: clears screen\r\n"
        "klaud restart: restarts kernel\r\n"
        "klaud math <expression>: the operating system's 4 function calculator\r\n"
        "klaud plot <expression>: plots given function (i.e. x^2, x+1)\r\n"
        "klaud fun-fact: get fun facts about Klaud!\r\n"
        "klaud random: klaud chooses a command for you!\r\n"
        "Commands with --help available: klaud memory, text-color, math, and plot\r\n"
        "Keys:\r\n"
        "Esc: makes new line without running the previous\r\n"
        "Enter: makes a new line while running the previous\r\n"
        "Up arrow: returns the previous command into current line\r\n"
        "Shift: same usage as normal\r\n"
        "Caps Lock: same usage as normal"
    );
    scroll(22);
}

void newLine(int newLines) {
    int i;
    for (i = 0; newLines > i; i++) {
        printf("\n");
    }
    printf("%s> ", findDirName(getCurrDir()));
    move_curs(strlen(findDirName(getCurrDir())));
}

void startUp() {
    clrscr();
    klaud_ascii();
    printf("\n                         Version -- 0.0.6: Oct 2023\n");
    printf("                             Welcome to KlaudOS\n");
}