<h1 align="center">
  <br>
  <br>
    Commands
  <br>
</h1>
<p align="center">
  <a href="#general">General</a> •
  <a href="#ascii-pictures">Ascii Pictures</a> •
  <a href="#math">Math</a> •
  <a href="#memory">Memory</a> •
  <a href="#sillygames">Silly/Games</a>
</p>

## General

### klaud
Every command starts with klaud

![klaud](../assets/klaud.png)

### klaud --help
General help. Shows all available commands.

![help](../assets/help.png)

### klaud echo
Works the same as echo command in Linux

![echo](../assets/echo.png.png)

### klaud clear

Clears screen. That's it.

![clear](../assets/blank.png)

input command: klaud clear

### klaud restart

Restarts the kernel after entering yes into the prompt.

![restart](../assets/klaudRestart.png)

### klaud startup

Without restarting the kernel takes the user back to the title page.

![startup](../assets/startup.png)

input command:
klaud startup

### klaud text-color
Command to change the text color. You can check klaud text-color --help to see which colors are supported.

![txtClr](../assets/textcolor.png)

input command:
klaud text-color purple

### klaud back-color
Command to change the background color. You can check klaud back-color --help to see which colors are supported.

![backClr](../assets/backColor.png)

input command:

klaud back-color light blue

### klaud text-color random
Klaud chooses what color your text is from the available colors listed in klaud text-color --help.

![txtClrRand](../assets/textColorRand2.png)

input command:

klaud text-color random

### klaud text-color default
Change text-color back to default (white). You could use klaud text-color white to do this but in my head this command is easier to use.

![txtClrDef](../assets/textColorDef.png)

input commands:

klaud text-color default

### klaud text-color --help
Help function for klaud text-color.

![txtClrHelp](../assets/textcolorHelp.png)

### klaud back-color --help
Help function for klaud back-color.

![backClrHelp](../assets/backColorHelp.png)

## Ascii Pictures

### klaud ascii
Prints ascii picture of klaud

![ascii](../assets/ascii.png)

### klaud shrine
Prints ascii picture of klaud with flowers, great for meditation

![shrine](../assets/shrine.png)

### klaud live-slug-reaction
Prints ascii picture of klaud actually reacting

![lsr](../assets/reaction.png)

## Math

### klaud math
Simple calculator with PEMDAS. Supports: 
* Addition
* Subtraction
* Multiplication
* Division
* Modulo
* Exponential
* Factorials

![math](../assets/math.png)

### klaud plot
A text-based plotting system. Used for plotting simple functions like y = x or y = x^2

![plot](../assets/plot.png)

input command: klaud plot x

### klaud plot point
Plots a point at a specified location. To add more points to plot run the command again. The point just plotted will be in the form of a 'X' while the previous points are in the form of a 'O'. To clear the plot run the klaud plot clear command.

There is also a line of best fit calculation below the plot (it's not great but it is what it is).

![point](../assets/plotpoint.png)
input commands (in this order): 

klaud plot point 0,0

klaud plot point 2,2

### klaud plot point random
Plots a point in a random place on the grid. This command operates the same as the klaud plot point command, so adding more points to the plot can be achieved with either this command or klaud plot point and clearing the plot is done with klaud plot clear.

You can also add more than one random point at a time by adding a number at the end of the command (i.e. 10 random points: klaud plot point random 10)

![plotRand](../assets/plotRand.png)

input commands:

klaud plot point random

### klaud plot clear

Clears the plot of saved points. 

![plotClear](../assets/plotClear.png)

input commands: 

klaud plot clear

### klaud math --help
Help function for klaud math

![mathHelp](../assets/mathHelp.png)

### klaud plot --help
Help function for klaud plot

![plotHelp](../assets/plotHelp.png)

## Memory

### klaud memory

Save values to memory. The first value in the command is the memory location (int) where the highest it can go is 10000 at the moment. The second value is the value you're saving.

![mem](../assets/memory.png)

### klaud remember

Read from memory from the memory location.

![remem](../assets/memRem.png)

### klaud free-bytes

Shows memory location of used bytes.

![freeBytes](../assets/freeBytes.png)

### klaud del

Clears data at given index range.

![memDel](../assets/memDel.png)

### klaud memory --help

Help command for klaud memory.

![memHelp](../assets/memHelp.png)

## Silly/Games

### klaud dice
Gamble with Klaud! The object of the game is to roll a higher number on the dice. If you do, you win the bet. You both start with 500 Republic Credits and every turn you have 3 options:
* place a bet
    * type a number to bet that amount
* place random bet
    * type random to bet a random amount
* leave game
    * type exit

![dice](../assets/dice.png)

### klaud rizz
Ever want to rizz up Klaud? If the answer is yes I have the command for you. Tell him whatever you want with a range of responses depending on how much he likes what you typed.

![rizz](../assets/klaudrizz.png)

![rizz1](../assets/klaudrizz1.png)

![rizz2](../assets/klaudrizz2.png)

input command: klaud rizz

### klaud haiku
Klaud makes a haiku for you!

![haiku](../assets/haiku.png)

### klaud random
Klaud chooses a command for you.

![rand](../assets/random.png)