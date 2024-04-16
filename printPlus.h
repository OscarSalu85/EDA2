#include <stdio.h>

//https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

//Message, F_RGB, B_RGB
void printColor(char message[],char F_RBG[], char B_RBG[]){
    printf( "\e[38;2;%sm" "\e[48;2;%sm" "%s" "\e[0m" "\n",F_RBG, B_RBG, message);
}
