#include "stdio.h"
#include "libs/disp.h"

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
    scroll(8);
}

void help() {
    puts(
        "Commands:\r\n"
        "klaud: Everything starts with Klaud.\r\n"
        "klaud ascii: prints ascii picture of Klaud.\r\n"
        "klaud echo <phrase>: returns whatever you say back\r\n"
        "klaud dice <bet value (int)>: play street dice with klaud\r\n"
        "klaud live-slug-reaction: ascii art of the 'live slug reaction' meme\r\n"
        "klaud shrine: Klaud shrine in ascii art for your Klaud worshiping needs\r\n"
        "klaud rizz: rizz up Klaud and see what he says\r\n"
        "klaud haiku: klaud makes a haiku! it wont be nice, he doesn't speak english\r\n"
        "klaud text-color <color>: changes the color of kernel text.\r\n"
        "klaud clear: clears screen\r\n"
        "klaud restart: restarts kernel\r\n"
        "klaud math <expression>: the operating system's 4 function calculator\r\n"
        "klaud plot <expression>: plots given function (i.e. x^2, x+1)\r\n"
        "klaud fun-fact: get fun facts about Klaud!\r\n"
        "Keys:\r\n"
        "Backspace: does nothing, i have not figured out backspace. sorry\r\n"
        "Esc: makes new line without running the previous\r\n"
        "Enter: makes a new line while running the previous\r\n"
        "Up arrow: prints the previous command into current line\r\n"
        "Shift: same usage as normal (uppercase not supported yet)"
    );
    scroll(21);
}