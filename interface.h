#include <stdio.h>
#include "structures.h"
#include "printPlus.h"
#include "dataR.h"
void displaySkills_New(Skills skill_list){
    
}

void printScenario(Scenario *Scene){
    printf("%s\n",Scene->name);
    printImage(Scene->image);
    printf("%s",Scene->description);
}

/*

int main(){
    Scenario Scene = extractScenario("Deck");
    printScenario(&Scene);
}

*/


