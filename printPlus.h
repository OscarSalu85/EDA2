#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include "structures.h"
#define CHAR_SIZE 32
#define BUFFER_SIZE 1024
#define BASE_SPEED 3000
#ifndef PRINTPLUS_H
#define PRINTPLUS_H
//https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

//Message, F_RGB, B_RGB
void printColor(char message[],char F_RBG[], char B_RBG[]){
    printf( "\e[38;2;%sm" "\e[48;2;%sm" "%s" "\e[0m",F_RBG, B_RBG, message);
}

void clearScreen() {
    printf("\033[2J\033[H");
    fflush(stdout); // Ensure the escape code is sent immediately
}
/*
fileImage: "name.txt"
file format:
(int)height (int)width
(char[])character (int)numberRepetions (char[])RGB_Background (char[])line_break --> NSL/SL 
...
*/
void printImage(char fileImage[]){
    FILE *f = fopen(fileImage,"r");
    int height;
    int width;
    int remaining;
    char character[CHAR_SIZE];
    int number;
    char RBG[CHAR_SIZE];
    char line_break[CHAR_SIZE];
    fscanf(f, "%d %d", &height, &width);
    remaining = height * width;
    while(remaining > 0){
        fscanf(f, "%s %d %s %s",character,&number,RBG,line_break);
        if(!strcmp(character,"N")) strcpy(character, "  ");
        for(int j = 0; j < number; j++){
            printColor(character,"255;0;0",RBG);
        }
        if(!strcmp(line_break,"SL")) printf("\n");
        remaining -= number;
    }
    fclose(f);
}

//Function for QOL text printing so that it's easier to follow what's going on for the user
void printText(const char *text, int printTime) {
    for (int i = 0; i < strlen(text); i++) {
        printf("%c", text[i]);
        fflush(stdout);  // Prints the character immediately
        usleep(printTime);   // Time in u seconds it takes for each character to be printed
    }
}

//Function provided by chatGPT to format the text so that it can be printed by the printtext function correctly
void printFormattedText(const char *format,int printTime ,...) {
    char buffer[BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, BUFFER_SIZE, format, args);
    va_end(args);
    printText(buffer, printTime);
}

//Function to print the scenario image and description on screen
void printScenario(Scenario *Scene){
    printImage(Scene->image);
    printFormattedText("%s",BASE_SPEED,Scene->description);
    sleep(3);
}

#endif