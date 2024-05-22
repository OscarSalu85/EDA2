#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "interface.h"
#include "queque.h"
#include "dataR.h"
#define BASE_HP 10
#define INITIAL_STATS 20
#define HP_PER_POINT 5 //Hp value given for every stat point invested
int main_menu(Data *data){
    int choice;
    load_data(data);
    printf("1.Continue      2.New Game     3.Configure \n");
    while(1)
    {
        printf("\nSelect your choice: ");
        scanf("%d", &choice);
        if(choice == 1){
            if(!isalpha(data->character->name[0])){
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
            if(strcmp(data->character->name, "") == 1){
                printf("\nNo saved data found, please start a new game\n");
            }
            else{
                printf("\nSaved data found, opening character configuration...\n");
                return 3;
            }
        }
    }
}


void configure_name(Data* data){
    int count = 0;
    int choice = 0;
    while (getchar() != '\n');
    data->character->name = malloc(MAX_CHAR_NAME * sizeof(char));

    if (data->character->name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    while (1)
    {
        count = 0;
        printf("\nSelect your new name: ");
        fgets(data->character->name, MAX_CHAR_NAME, stdin);
        printf("%s", data->character->name);

        size_t len = strlen(data->character->name);
        if (len > 0 && data->character->name[len - 1] == '\n') {
            data->character->name[len - 1] = '\0';
        }

        if(strcmp(data->character->name, "")){
            if(data->character->name[0] == '\0') {
            printf("\nName cannot be empty.\n");
            continue;
        }
            else{
                for(int i=0;i<strlen(data->character->name);i++){
                    if (!isalpha(data->character->name[i]) && data->character->name[i] != ' '){
                        printf("\nThe name can only contain letters from the alphabet, try again.\n");
                        count = 1;
                        break;
                    }
                    
                }
                if(count == 0){
                    while(1){
                        printf("\nName Selected: %s\nConfirm name? (1 = yes, 0 = no): ", data->character->name);
                        if (scanf("%d", &choice) != 1) {
                            printf("\nNot a valid option, try again.\n");
                            // Clear invalid input from buffer
                            while (getchar() != '\n');
                            continue;
                        }
                        
                        // Clear the newline character left in the buffer
                        while (getchar() != '\n');

                        if (choice == 1) {
                            return;
                        } else if (choice == 0) {
                            break;
                        } else {
                            printf("\nNot a valid option, try again.\n");
                        }
                    }
                    
                }
            }
        }
        
    }
}


void configure_stats(Data* data){
    data->character->atk = 0;
    data->character->def = 0;
    data->character->hp = 10;
    int input;
    int current_points = 20;
    while(current_points > 0){
            printf("\nAvailable SP(Stat Points):%d SP",current_points);
            printf("\nInvested SP:    1.Atk:%d      2.Def:%d      3.HP:%d", data->character->atk,data->character->def,(data->character->hp-BASE_HP) / HP_PER_POINT);
            printf("\nSelect which stat you want to invest in: ");
            scanf("%d", &input);
            if(input < 1 || input>3){
                printf("\nSelect a valid element to invest.");
            }
            else{
                int input2;
                printf("\nHow many stat points do you want to invest in this stat?(Press 0 to not invest in this stat for now): ");
                scanf("%d", &input2);
                if(input2 > current_points || input2 < 0){
                    printf("\nNot enough stat points available.");
                }
                else if(input2 > 0){
                    if(input == 1){
                        data->character->atk += input2;
                    }
                    else if(input == 2){
                        data->character->def += input2;
                    }
                    else if(input == 3){
                        data->character->hp += input2 * HP_PER_POINT;
                    }
                    current_points -= input2;
                }
            }
        } 
        return;
}


void print_skill_list(Skills *skill_list){
    for(int i=0; i<SKILL_MAX; i++){
        printf("\n%d.SKILL Nº%d: %s", i+1, i+1, skill_list[i].name);
    }
}

void configure_skills(Data* data){
    int confirm;
    int selected_skill;
    int repeat;
    Skills *skill_list;
    skill_list = get_skill_data();
    print_skill_list(skill_list);
    while(1){
        for(int i=0; i<MAX_SKILLS;i++){
            while(1){
                repeat = 0;
                printf("\nSelect your skill nº%d: ", i+1);
                scanf("%d",&selected_skill);
                selected_skill--;
                if(selected_skill<0 || selected_skill >SKILL_MAX){
                    printf("\nNo valid skill was selected, please choose a valid skill.");
                }
                else{
                    for(int j = 0; j<MAX_SKILLS;j++){
                        if(skill_list[selected_skill].name ==  data->character->skill[j]->name){
                            printf("\nYou have already chosen thiss kill, select a different one.");
                            repeat = 1;
                        }
                    }
                    if(repeat == 1){
                        continue;
                    }
                    printf("\nSkill Name: %s", skill_list[selected_skill].name);
                    printf("\n%s", skill_list[selected_skill].description);
                    printf("\nSkill Duration: %d turns", skill_list[selected_skill].duration);
                    printf("\nDamage: %d", skill_list[selected_skill].damage);
                    printf("\nSelf modifiers: (%d ATK, %d DEF, %d HP)", skill_list[selected_skill].modifiers[0],skill_list[selected_skill].modifiers[1],skill_list[selected_skill].modifiers[2]);
                    
                    printf("\nDo you want to equip this skill as your skill nº%d (1.Yes 2.No)", i);
                    scanf("%d", &confirm);
                    if(confirm == 1){
                        *data->character->skill[i] = skill_list[selected_skill];
                        printf("\n%s added to your character's skill set", skill_list[selected_skill].name);
                    }
                    else{
                        continue;
                    }
                    break;
                }
            }
        }
        printf("\n%s's current skill set: ", data->character->name);
        for(int i = 0; i<MAX_SKILLS; i++){
            printf("\n%s", data->character->skill[i]->name);
        }
        
        printf("\nConfirm skill set?\n1.Yes\n2.No\n" );
        scanf("%d", &confirm);
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

int selectTarget(Enemy *enemies){
    int num_alive = 0;
    int num_enemies = 0;
    for(int i = 0; i < MAX_ENEMIES;i++){
        if(enemies[i].name != NULL){
            num_enemies++;
        }
    }
    for(int i = 0; i < num_enemies - 1; i++){
        if(enemies[i].hp > 0){
            num_alive++;
            printf("\n%d.%s(hp:%d,atk:%d,def:%d)",i+1,enemies[i].name,enemies[i].hp,enemies[i].atk,enemies[i].def);
        }
    }
    if(num_alive == 0) return -1;
    printf("\nChose option:");
    int opt = 0;
    while(opt < 1 || opt > num_enemies){
        scanf("%d", &opt);
        if(0 < opt < num_enemies && enemies[opt -1].hp > 0) return opt-1;
        opt = 0;
        printf("\nChose valid option:");
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
    int random = rand()%(sizeof(current_enemy->skill)/sizeof(Skills));
    *skill = current_enemy->skill[random];
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
    Queue *queue = malloc(sizeof(Queue));
    createQueue(character,enemies, queue);
    while(active && queue->first != NULL){
        //printCombat(enemies);
        //Character
        if(queue->first->type == 0){
            printf("\nPlayer Turn:");
            playerTurn(queue->first, enemies, character);
        }
        else{
            printf("\nEnemy Turn:");
            active = enemyTurn(queue->first, character);
        } 
        
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

void create_data(Data **data){
    // Allocate memory for Data structure
    *data = (Data*)malloc(sizeof(Data));
    if (*data == NULL){
        //Returns if memory could not be allocated correctly
        return;
    }

    //Allocate memory for Character structure
    (*data)->character = (Character*)malloc(sizeof(Character));
    if ((*data)->character == NULL){
        //Returns if memory could not be allocated correctly
        free(*data);
        return;
    }

    //Allocate memory for Character's skills 
    for (int i = 0; i < MAX_SKILLS; i++){
        (*data)->character->skill[i] = (Skills*)malloc(sizeof(Skills));
        if ((*data)->character->skill[i] == NULL){
            //Returns if memory could not be allocated correctly
            for (int j = 0; j < i; j++){
                free((*data)->character->skill[j]);
            }
            free((*data)->character);
            free(*data);
            return;
        }
    }

    // Allocate memory for the current scenario
    (*data)->current_scenario = (Scenario*)malloc(sizeof(Scenario));
    if ((*data)->current_scenario == NULL) {
        // Handle memory allocation failure
        for (int i = 0; i < MAX_SKILLS; i++) {
            free((*data)->character->skill[i]);
        }
        free((*data)->character);
        free(*data);
        return;
    }
}

Scenario* allocate_scenarios(int num_scenarios) {
    Scenario* scenarios = malloc(num_scenarios * sizeof(Scenario));
    if (scenarios == NULL) {
        fprintf(stderr, "Memory allocation failed for scenarios\n");
        exit(1);
    }

    for (int i = 0; i < num_scenarios; i++) {
        scenarios[i].name = malloc(MAX_CHAR_NAME * sizeof(char));
        if (scenarios[i].name == NULL) {
            fprintf(stderr, "Memory allocation failed for scenario name\n");
            exit(1);
        }

        scenarios[i].description = malloc(MAX_CHAR_DESC * sizeof(char));
        if (scenarios[i].description == NULL) {
            fprintf(stderr, "Memory allocation failed for scenario description\n");
            exit(1);
        }

  

        scenarios[i].decision = malloc(sizeof(Decision));
        if (scenarios[i].decision == NULL) {
            fprintf(stderr, "Memory allocation failed for scenario decision\n");
            exit(1);
        }

        for (int j = 0; j < MAX_OPTIONS; j++) {
            scenarios[i].decision->options[j] = malloc(sizeof(Option));
            if (scenarios[i].decision->options[j] == NULL) {
                fprintf(stderr, "Memory allocation failed for decision option\n");
                exit(1);
            }

            for (int k = 0; k < MAX_ENEMIES; k++) {
                scenarios[i].decision->options[j]->enemies[k] = malloc(sizeof(Enemy));
                if (scenarios[i].decision->options[j]->enemies[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for enemy\n");
                    exit(1);
                }

                scenarios[i].decision->options[j]->enemies[k]->name = malloc(MAX_CHAR_NAME * sizeof(char));
                if (scenarios[i].decision->options[j]->enemies[k]->name == NULL) {
                    fprintf(stderr, "Memory allocation failed for enemy name\n");
                    exit(1);
                }
            }
        }

        scenarios[i].next_scenario_name_1 = malloc(MAX_CHAR_NAME * sizeof(char));
        if (scenarios[i].next_scenario_name_1 == NULL) {
            fprintf(stderr, "Memory allocation failed for next scenario name 1\n");
            exit(1);
        }

        scenarios[i].next_scenario_name_2 = malloc(MAX_CHAR_NAME * sizeof(char));
        if (scenarios[i].next_scenario_name_2 == NULL) {
            fprintf(stderr, "Memory allocation failed for next scenario name 2\n");
            exit(1);
        }
    }

    return scenarios;
}


//CREATES A NEW GAME
void new_game(Data *data){
    if(data != NULL){
        free(data); //Frees saved data
    }
    create_data(&data);
    save_data(data, 0);//Overwrites save file with empty file, second parameter = 0 --> Delete data
    configure_name(data);
    configure_stats(data);
    configure_skills(data);
    //printf("%s",data->character->name);
    char* scenario = "Deck";
    data->current_scenario->name = scenario;
    save_data(data,1); //Saves the configured data
}

void continue_game(Data *data){
    printf("\nSTARTING MAIN LOOP");
    mainLoop(data);
};












