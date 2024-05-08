
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "structures.h"
#include "interface.h"
#include "queque.h"

int main_menu(Data *data){
    printf("1.Continue      2.Save Game     3.Configure \n");
    while (1)
    {
        int choice = scanf("Select your option: ");
        if(choice == 1){
            get_save_data(data);
            if(data == NULL){
                printf("\nNo saved data found, please start a new game\n");
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
    char name[MAX_CHAR] = scanf("%s");
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
    mainLoop(data);
};

void playerTurn(){
    
}

void enemyTurn(){
    
}


int combat(Character *character, Enemy *enemies){
    int active = 1;
    Queue queue = createQueue(character,enemies);
    while(active){
        //Turn Player
        playerTurn();
        //Turn Enemy
        enemyTurn();
        if(character->hp <= 0 || queue.last == NULL) return 0;
    }
    return 1;
}

int Decision(Data *data, Scenario scene){
    for(int desc = 0; desc < 3;desc++){
        Decisions currentDesc = scene.decision[desc];
        int option = -1;
        while(option < 0 ||option > currentDesc.n_options){
            printf("%s",currentDesc.question);
            scanf("%d",option);
        }
        Option currentOpt = currentDesc.options[option];
        printf("%s",currentOpt.r_text);
        int win = combat(data->character, currentOpt.enemies);
        if(win != 0){
            printf("%s",currentOpt.n_text);
        }
        else return 0;
    }
}

void mainLoop(Data *data){
    Scenario currentScene = extractScenario(data->current_scenario);
    printScenario(currentScene.name);

}

