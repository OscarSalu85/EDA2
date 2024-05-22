#include <stdio.h>
#include "structures.h"
#include "game.h"
#include "cJSON.c"

int main(){
    Data *data;
    create_data(&data);
    Scenario *sceneNodes = allocate_scenarios(2);
    sceneNodes = get_scenario_nodes(sceneNodes);
    while(1){
        //Menu inicial
        int main_menu_choice = main_menu(data);
        if(main_menu_choice == 1){
            for(int i = 0; i < MAX_SKILLS;i++){
                load_Skill(data->character->skill[i]);
                printf("%s-->%d",data->character->skill[i]->name,data->character->skill[i]->damage);
            }
            //Test combat
            Enemy enemies[1];
            Enemy enemy1;
            enemy1.atk = 1;
            enemy1.def = 1;
            enemy1.hp = 1;
            enemy1.name = "cultist";
            enemy1.skill[0] = *data->character->skill[0];
            enemy1.skill[1] = *data->character->skill[1];
            enemy1.skill[2] = *data->character->skill[2];
            enemy1.skill[3] = *data->character->skill[3];
            enemies[0] = enemy1;
            int sol = combat(data->character,enemies);
            printf("%d",sol);
            //continue_game(data);
        }
        else if(main_menu_choice == 2){
            new_game(data);
        }
        else if(main_menu_choice == 3){
            configure_menu(data);
        }
    }
}


//gcc -o main main.c cJSON.c