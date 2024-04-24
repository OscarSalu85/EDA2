#include <stdio.h>
#include <string.h>
#include "printPlus.h"
/*
First convert PNG to a RGB txt
1.https://onlinepngtools.com/convert-png-to-rgb-values
2.Convert
3.Copy into .txt
*/

//(char[])Source-->"name.txt" (char[])Destination -->"name.txt" (int)height (int)width
void convert(char img[], char dest[],int height, int width){
    FILE *image = fopen(img, "r");
    FILE *text = fopen(dest, "w");
    int red, green, blue;
    int l_red = -1, l_green = -1, l_blue = -1;
    int current = 0;
    int current_same = 0;
    
    fprintf(text,"%d %d\n",height,width);
    fprintf(text,"\n");

    while(!feof(image)){
            fscanf(image, "%d, %d, %d, ",&red, &green, &blue);
            if(l_red == -1) l_red = red;
            if(l_green == -1) l_green = green;
            if(l_blue == -1) l_blue = blue;
            if(red == l_red && blue == l_blue && green == l_green){
                if(current >= width){
                    fprintf(text,"N %d %d;%d;%d SL\n", current_same, red,green,blue);
                    fprintf(text,"\n");
                    current = 0;
                    current_same = 1;
                }
                else{
                    current_same += 1;
                }
            }
            else{
                if(current >= width){
                    fprintf(text,"N %d %d;%d;%d SL\n", current_same, l_red,l_green,l_blue);
                    fprintf(text,"\n");
                    current = 0;
                }
                else{
                    fprintf(text,"N %d %d;%d;%d NSL\n", current_same, l_red,l_green,l_blue);
                }
                current_same = 1;
            }

            l_red = red;
            l_green = green;
            l_blue = blue;

            current++;
    }
    fprintf(text, "N %d %d;%d;%d SL\n", current_same, red, green, blue);
    fclose(image);
    fclose(text);
}

int main(){
    convert("Image.txt","Final.txt",12,16);
    printImage("Final.txt");
    getchar();
}