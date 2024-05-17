#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "interface.h"
#include "queque.h"
#include "dataR.h"
#define BASE_HP 5
#define INITIAL_STATS 20
#define HP_PER_POINT 5 //Hp value given for every stat point invested
int main_menu(Data *data){
    printf("1.Continue      2.New Game     3.Configure \n");
    while (1)
    {
        load_data(data);
        int choice = scanf("Select your option: ");
        if(choice == 1){
            
            if(strcmp(data->character->name, "")){
                printf("\nNo saved data found, please start a new game\n");
            }
            else{
                printf("Saved data found, continuing game...\n");
                return 1;
            }
        }
        else if(choice == 2){
            return 2;
        }
        else if(choice == 3){
            if(strcmp(data->character->name, "")){
                printf("\nNo saved data found, please start a new game\n");
            }
            else{
                printf("Saved data found, opening character configuration...\n");
                return 3;
            }
        }
    }
}


void configure_name(Data* data){
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
            else if(strcmp(data->character->name, "")){
                printf("Name can not be empty. \n");
                int count = 1;
                break;
            }
        }
        if(count == 0){
            break;
        }
    }
    return;
}


void configure_stats(Data* data){
    int starting_points = INITIAL_STATS;
    data->character->atk = 0;
    data->character->def = 0;
    data->character->hp = 10;
    
    int current_points = INITIAL_STATS;
    while(current_points != 0){
            printf("\nAvailable SP (Stat Points) : %d",current_points);
            printf("\nInvested SP:    1.Atk:%d      2.Def:%d      3.HP:%d", data->character->atk,data->character->def,(data->character->hp-BASE_HP) / HP_PER_POINT);
            int input = scanf("\nSelect which stat you want to invest in: ");
            if(input < 1 || input>3){
                printf("\nSelect a valid element to invest.");
            }
            else{
                int input2 = scanf("\nHow many stat points do you want to invest in this stat?(Press 0 to not invest in this stat for now)");
                if(input2 > current_points || input2 < 0){
                    printf("\nNot enough stat points available.");
                }
                else if(input2 > 0){
                    if(input == 1){
                        data->character->atk += input;
                    }
                    else if(input == 2){
                        data->character->def += input;
                    }
                    else if(input == 3){
                        data->character->hp += input * HP_PER_POINT;
                    }
                }
            }
        } 
        return;
}

void print_skill_list(Skills *skill_list[SKILL_DATA_SIZE]){
    for(int i=0; i<SKILL_DATA_SIZE; i++){
        printf("\n%dSKILL Nº%d: %s", i+1, i+1, skill_list[i]->name);
    }
}

void configure_skills(Data* data){
    Skills *skill_list[SKILL_DATA_SIZE];
    get_skill_data(skill_list);
    print_skill_list(skill_list);
    while(1){
        for(int i=0; i<MAX_SKILLS;i++){
            while(1){
                printf("\nSelect your skill nº%d: ", i+1);
                int selected_skill = scanf("") -1;
                if(selected_skill<0 || selected_skill >19){
                    printf("\nNo valid skill was selected, please choose a valid skill.");
                }
                else{
                    data->character->skill[i] = skill_list[selected_skill];
                    printf("\nSkill Name: %s", skill_list[selected_skill]->name);
                    printf("\n%s", skill_list[selected_skill]->description);
                    printf("\nSkill Duration: %d turns", skill_list[selected_skill]->duration);
                    printf("\nDamage: %d", skill_list[selected_skill]->damage);
                    printf("\nSelf modifiers: (%d ATK, %d DEF, %d HP)", skill_list[selected_skill]->modifiers[0],skill_list[selected_skill]->modifiers[1],skill_list[selected_skill]->modifiers[2]);
                    int confirm_skill = 

                    printf("\n%s added to your character's skill set", skill_list[selected_skill]->name);
                }
            }
        }
        printf("\n%s's current skill set: ", data->character->name);
        for(int i = 0; i<MAX_SKILLS; i++){
            printf("\n%s", data->character->skill[i]->name);
        }
        int confirm = scanf("\nConfirm skill set?\n1.Yes\n2.No");
        if(confirm == 1){
            return;
        }
    }
}

void configure_menu(Data *data){
    while(1){
        printf("\n1.Change Name\n2.Change stat allocation\n3.Change skill set\nBack to Main Menu");
        int choice = scanf("\nSelect an option: ");
        if(choice == 1){
            configure_name(data);
        }
        else if(choice == 2){
            configure_stats(data);
        }
        else if(choice == 3){
            configure_skills(data);
        }
        else if(choice == 4){
            main_menu(data);
        }
        else{
            printf("No valid option was chosen, please try again.");
        }
    }
};

void selectSkill(Character *character, Skills *skill){
    for(int i = 0; i < 4;i++){
        printf("\n%d.%s\n:",i+1,character->skill[i]->name);
        printf("   -Desc:%s\n",character->skill[i]->description);
        printf("   -Dur:%d\n",character->skill[i]->duration);
        printf("   -Mod: (%d ATK, %d DEF, %d HP)\n",character->skill[i]->modifiers[0],character->skill[i]->modifiers[1],character->skill[i]->modifiers[2]);
    }
    printf("Chose option:");
    int opt = -1;
    while(opt < 1 || opt > 5){
        scanf("%d", &opt);
        if(0 < opt && opt < 5) {
            skill = character->skill[opt-1];
            return;
        }
        opt = 0;
        printf("Chose valid option:");
    }
}

int selectTarget(Enemy *enemies[MAX_ENEMIES]){
    int num_alive = 0;
    int num_enemies = sizeof(&enemies)/sizeof(Enemy);
    for(int i = 0; i < num_enemies; i++){
        if(enemies[i]->hp > 0){
            num_alive++;
            printf("\n%d.%s(hp:%d,atk:%d,def:%d)",i+1,enemies[i]->name,enemies[i]->hp,enemies[i]->atk,enemies[i]->def);
        }
    }
    if(num_alive == 0) return -1;
    printf("Chose option:");
    int opt = 0;
    while(opt < 1 || opt > num_enemies){
        scanf("%d", &opt);
        if(0 < opt < num_enemies && enemies[opt -1]->hp > 0) return opt-1;
        opt = 0;
        printf("Chose valid option:");
    }
    return opt-1;
}


void playerTurn(Turn *turn, Enemy *enemies[MAX_ENEMIES], Character *character){
   if(enemies != NULL){
        Enemy *target; 
        int chosen_target_index = selectTarget(enemies);
        if(chosen_target_index != -1){
            target = enemies[chosen_target_index];
            Skills *skill;
            selectSkill(character,skill);
            //Attack
            int damage = 0;
            if(target->def > 0) damage = ((character->atk * skill->damage) / target->def);
            else damage = (character->atk * skill->damage);
            target->hp = target->hp - damage;
            //Modifiers
        }
   }
}

void selectEnemySkill(Enemy *current_enemy, Skills *skill){
    //Select skill randomly
}


int enemyTurn(Turn *turn, Character *character){
    Enemy *current_enemy = turn->enemy;
    Skills *skill;
    selectEnemySkill(current_enemy,skill);
    //Atack
    int damage = 0;
    if(character->def > 0) damage = ((current_enemy->atk * skill->damage) / character->def);
    else damage = (current_enemy->atk * skill->damage);
    character->hp = character->hp - damage;
    if (character->hp <= 0) return 0;
    return 1;
}

int combat(Character *character, Enemy *enemies[MAX_ENEMIES]){
    int active = 1;
    Queue *queue;
    createQueue(character,enemies, queue);
    while(active && queue->first != NULL){
        //Character
        if(queue->first->type == 0){
            playerTurn(queue->first, enemies, character);
        }
        else active = enemyTurn(queue->first, character);
        
        if(character->hp <= 0 || queue->first == NULL) return 0;
    }
    return 1;
}

int Decision(Data *data, Scenario scene){
    
    Decisions *currentDesc = scene.decision;
    int option = -1;
    while(option < 0 ||option > currentDesc->n_options){
        printf("%s",currentDesc->question);
        scanf("%d",&option);   
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


void mainLoop(Data *data){
    Scenario *currentScene = data->current_scenario;
    int active = 1;
    while(active){
        printScenario(currentScene);
        int active = Decision(data,*currentScene);

    }
}

//FUNCTIONS TO ALLOCATE MEMORY DATA FOR THE 
void create_data(Data* data){
    //ALLOCATES MEMORY FOR DATA
    data = malloc(sizeof(Data));
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
}



//CREATES A NEW GAME
void new_game(Data *data){
    if(data != NULL){
        free(data); //Frees saved data
    }
    create_data(data);
    save_data(data);//Overwrites save file with empty file
    configure_name(data);
    configure_stats(data);
    configure_skills(data);
}

void continue_game(Data *data){
    mainLoop(data);
};












