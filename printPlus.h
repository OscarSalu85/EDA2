#include <stdio.h>
#include <string.h>

#define CHAR_SIZE 30

//https://en.wikipedia.org/wiki/ANSI_escape_code#Colors

//Message, F_RGB, B_RGB
void printColor(char message[],char F_RBG[], char B_RBG[]){
    printf( "\e[38;2;%sm" "\e[48;2;%sm" "%s" "\e[0m",F_RBG, B_RBG, message);
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

void printCombatImage(char *FileImage[MAX_ENEMIES], int num){
    FILE *files[MAX_ENEMIES];
    //Print line by line, Jump after line completed
    for(int i = 0; i< num; i++){
        if(FileImage[i] != NULL){
            char *name = FileImage[i];
            strcat(name,".txt");
            files[i] = fopen(name,"r");
        }
        else{
            files[i] = fopen("NULL.txt","r");
        }
    }

}