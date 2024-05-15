#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "interface.h"
#include "queque.h"
#include "dataR.h"

int main_menu(Data *data){
    printf("1.Continue      2.New Game     3.Configure \n");
    while (1)
    {
        load_data(data);
        int choice = scanf("Select your option: ");
        if(choice == 1){
            
            if(data->character->name == ""){
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
    while (1)
    {
        int count = 0;
        printf("\nSelect your new name: ");
        scanf("\n%s",data->character->name);
        for(int i=0;i<strlen(data->character->name);i++){
            if(isalpha(data->character->name[i]) == 0){
                printf("The name can only contain letters from the alphabet, try again.\n");
                int count = 1;
                break;
            }
            else if(data->character->name == ""){
                printf("Name can not be empty. \n");
                int count = 1;
                break;
            }
        }
        if(count == 0){
            break;
        }
        
    }

    for(int i=0; i<MAX_SKILLS;i++){
        int selected_skill = scanf("\nSelect your skill nº%d", i+1);
        //data->character->skill[i] = skill_list[selected_skill];
    }
};

void mainLoop(Data *data){
    Scenario *currentScene = data->current_scenario;
    int active = 1;
    while(active){
        printScenario(currentScene->name);
        int active = Decision(data,*currentScene);

    }
}

//CREATES A NEW GAME
void new_game(Data *data){
    if(data != NULL){
        free(data); //Frees saved data
    }
    data = create_data();
    save_data(data);
    configure(data); //Configures new character and skills
}

void continue_game(Data *data){
    mainLoop(data);
};

Enemy selectTarget(Enemy *enemies){
    int num_alive = 0;
    int num_enemies = sizeof(enemies)/sizeof(Enemy);
    for(int i = 0; i < num_enemies; i++){
        if(enemies[i].hp > 0){
            num_alive++;
            printf("\n%d.%s(hp:%d,atk:%d,def:%d)",i+1,enemies[i].name,enemies[i].hp,enemies[i].atk,enemies[i].def);
        }
    }
    if(num_alive == 0) return;
    printf("Chose option:");
    int opt = 0;
    while(opt < 1 || opt > num_enemies){
        scanf("%d", opt);
        if(0 < opt < num_enemies && enemies[opt -1].hp > 0) return enemies[opt - 1];
        opt = 0;
        printf("Chose valid option:");
    }
}

Skills selectSkill(Character *character){
    for(int i = 0; i < 4;i++){
        printf("\n%d.%s\n:",i+1,character->skill[i].name);
        printf("   -Desc:%s\n",character->skill[i].description);
        printf("   -Dur:%d\n",character->skill[i].duration);
        printf("   -Mod:%s\n",character->skill[i].modifiers);
    }
    printf("Chose option:");
    int opt = -1;
    while(opt < 1 || opt > 5){
        scanf("%d", opt);
        if(0 < opt < 5) return character->skill[opt - 1];
        opt = 0;
        printf("Chose valid option:");
    }
}

void playerTurn(Turn *turn, Enemy *enemies, Character *character){
   if(enemies != NULL){
        Enemy target = selectTarget(enemies);
        Skills skill = selectSkill(character);
        //Attack
        int damage = 0;
        if(target.def > 0) damage = ((character->atk * skill.damage) / target.def);
        else damage = (character->atk * skill.damage);
        target.hp = target.hp - damage;
        //Modifiers
   }
}

Skills selectEnemySkill(Enemy *current_enemy){
    Skills *skill;
    //Select skill randomly
    return *skill;
}

int enemyTurn(Turn *turn, Character *character){
    Enemy *current_enemy = turn->enemy;
    Skills *skill = selectEnemySkill(current_enemy);
    //Atack
    int damage = 0;
    if(character->def > 0) damage = ((current_enemy->atk * skill->damage) / character->def);
    else damage = (current_enemy->atk * skill->damage);
    character->hp = character->hp - damage;
    if (character->hp <= 0) return 0;
    return 1;
}

int combat(Character *character, Enemy *enemies){
    int active = 1;
    Queue queue = createQueue(character,enemies);
    while(active && queue.first != NULL){
        //Character
        if(queue.first->type == 0){
            playerTurn(queue.first, enemies, character);
        }
        else active = enemyTurn(queue.first, character);
        
        if(character->hp <= 0 || queue.first == NULL) return 0;
    }
    return 1;
}

int Decision(Data *data, Scenario scene){
    
    Decisions *currentDesc = scene.decision;
    int option = -1;
    while(option < 0 ||option > currentDesc->n_options){
        printf("%s",currentDesc->question);
        scanf("%d",option);
    }
    Option *currentOpt = currentDesc->options[option];
    printf("%s",currentOpt->r_text);
    int win = combat(data->character, currentOpt->enemies);
    if(win != 0){
        printf("%s",currentOpt->n_text);
        return 1;
    }
    else return 0;
    
}

//FUNCTIONS TO ALLOCATE MEMORY DATA FOR THE 
Data* create_data(){
    //ALLOCATES MEMORY FOR DATA
    Data *data = malloc(sizeof(Data));
    if(data == NULL){
        return;
    }

    //ALLOCATES MEMORY FOR THE CHARACTER
    data->character = malloc(sizeof(Character));
    if(data->character == NULL){
        free(data);
        return;
    }

    //ALLOCATES MEMORY FOR CHARACTER SKILLS BY ITERATING THROUGH EACH ONE
    for (int i = 0; i < 4; i++) {
        data->character->skill[i] = malloc(sizeof(Skills));
        if (data->character->skill[i] == NULL) {
            // Handle memory allocation failure
            // Free previously allocated memory
            for (int j = 0; j < i; j++) {
                free(data->character->skill[j]);
            }
        }
    }

    //ALLOCATES MEMORY FOR THE CURRENT SCENARIO IN THE DATA
    data->current_scenario = malloc(sizeof(Scenario));
    if(data->current_scenario == NULL){
        free(data);
        return;
    }
    return data;
}

