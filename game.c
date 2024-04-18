#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "structures.h"

//CREATES A NEW GAME
new_game(Data *data){
    if(data != NULL){
        free(data); //Frees saved data
    }
    data = malloc(sizeof(Data)); //Creates new data in memory
    configure(data); //Configures new character and skills
}

void configure(Data *data){
    char name[MAX_CHAR] = scanf("\nSelect your new name: ");
    strcpy(data->character->name, name);

    for(int i=0; i<MAX_SKILLS;i++){
        int selected_skill = scanf("\nSelect your skill nº%d", i+1);
        data->skills[i]; // = skill_list[selected_skill];
    }
};

void continue_game(Data *data){

};