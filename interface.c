#include <stdio.h>
#include "structures.h"
#include "printPlus.h"
#include "dataR.h"
void displaySkills_New(Skills skill_list){
    
}

int main(){
    Scenario Scene = extractScenario("Deck");
    printf("%s\n",Scene.name);
    printImage(Scene.image);
    printf("%s",Scene.description);
}
