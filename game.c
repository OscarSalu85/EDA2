
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "structures.h"

int main_menu(Data *data){
    printf("1.Continue      2.Save Game     3.Configure \n");
    while (1)
    {
        int choice = scanf("Select your option: ");
        if(choice == 1){
            if(data == NULL){
                printf("\nNo saved data found\n");
            }
            else{
                printf("Saved data found, continuing game...");
                return 1;
            }
        }
        else if(choice == 2){
            return 2;
        }
        else if(choice == 3){
            return 3;
        }
    }
    
}

void configure(Data *data){
    printf("\nSelect your new name: ");
    char name[MAX_CHAR];
    scanf("%s",name);
    //char name[MAX_CHAR] = scanf("\nSelect your new name: ");
    strcpy(data->character->name, name);

    for(int i=0; i<MAX_SKILLS;i++){
        int selected_skill = scanf("\nSelect your skill nº%d", i+1);
        data->skills[i]; // = skill_list[selected_skill];
    }
};

//CREATES A NEW GAME
void new_game(Data *data){
    if(data != NULL){
        free(data); //Frees saved data
    }
    data = malloc(sizeof(Data)); //Creates new data in memory
    configure(data); //Configures new character and skills
}

void continue_game(Data *data){
};
