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

void printCombat(Enemy *enemies[MAX_ENEMIES]){
    char *name_enemies[MAX_ENEMIES];
    int num_enemies = sizeof(enemies)/sizeof(Enemy);
    for(int i = 0;i< num_enemies;i++){
        if(enemies[i] != NULL && enemies[i]->hp){
            name_enemies[i] = enemies[i]->name;
        }
        else name_enemies[i] = NULL;
    }
    printCombatImage(name_enemies,num_enemies);
}

/*

int main(){
    Scenario Scene = extractScenario("Deck");
    printScenario(&Scene);
}

*/


