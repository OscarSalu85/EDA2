#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "queue.h"
#include "dataR.h"
#include "printPlus.h"
#define HP_PER_POINT 2
#ifndef COMBAT_H
#define COMBAT_H

//Function to select skill while in combat
Skills* selectSkill(Character *character, Time_Strike *time_strk){
    int opt;
    while(1){
        //Prints all available skills
        for(int i = 0; i < 4;i++){
            printFormattedText("\n%d.%s:\n",BASE_SPEED,i+1,character->skill[i]->name);
        }
        //Asks for user input
        printText("\nChoose option (press 5 if you want to see the skill information or 6 for the skill time strike): ",BASE_SPEED);
        scanf("%d", &opt);
        while (getchar() != '\n');
        if(0 < opt && opt < 5) {
            return character->skill[opt-1];
        }
        //Shows the details of every skill on screen
        else if(opt == 5){
            clearScreen();
            for(int i = 0; i < 4;i++){
                printFormattedText("\n%d.%s:\n", BASE_SPEED, i+1,character->skill[i]->name);
                printFormattedText("   -Desc:%s\n",BASE_SPEED,character->skill[i]->description);
                printFormattedText("   -Dur:%d\n",BASE_SPEED,character->skill[i]->duration);
                printFormattedText("   -Dam:%d\n",BASE_SPEED,character->skill[i]->damage);
                printFormattedText("   -Mod: (%d ATK, %d DEF, %d HP)\n",BASE_SPEED,character->skill[i]->modifiers[0],character->skill[i]->modifiers[1],character->skill[i]->modifiers[2]);
                
            }  

            printText("\nChoose option: ",BASE_SPEED);
            scanf("%d", &opt);
            if(0 < opt && opt < 5) {
                clearScreen();
                //Returns the selected skill
                return character->skill[opt-1];
            }
        }
        else if(opt == 6 && time_strk->num_skills != 0 && time_strk->used == 0){
            printText("\nTime Strike:",BASE_SPEED);
            int num_skill = rand()%time_strk->num_skills;
            Skills *selected_skill = &time_strk->array[num_skill];
            Skills *upgraded_skill = malloc(sizeof(Skills));
            upgraded_skill->name = selected_skill->name;
            upgraded_skill->description = selected_skill->description;
            upgraded_skill->duration = selected_skill->duration * 2;
            upgraded_skill->damage= selected_skill->damage *2;
            upgraded_skill->modifiers[0] = selected_skill->modifiers[0] * 2;
            upgraded_skill->modifiers[1] = selected_skill->modifiers[1] * 2;
            upgraded_skill->modifiers[2] = selected_skill->modifiers[2] * 2;
            time_strk->used = 1;
            printFormattedText("\nYou have used %s with all the stats x2", BASE_SPEED,upgraded_skill->name);
            sleep(2);
            return upgraded_skill;
        }
        else if(opt == 6){
            printText("\nYou can't use time strike now, choose a valid option",BASE_SPEED);
        }
        else{
            printText("\nChoose a valid option.",BASE_SPEED);
        }
        //Cleans the input buffer and tries again
        while (getchar() != '\n');
        opt = 0;        
        sleep(1);
        clearScreen();
    }
}

//Function to select the enemy to attack
int selectTarget(Enemy *enemies[MAX_ENEMIES],int num_enemies){
    int num_alive = 0;
    int opt = 0;
    while(1){
        clearScreen();
        //Prints the available enemies to target
        for(int i = 0; i < num_enemies; i++){
            if(enemies[i]->hp > 0){
                num_alive++;
                printFormattedText("\n%d.%s(hp:%d,atk:%d,def:%d)",BASE_SPEED,i+1,enemies[i]->name,enemies[i]->hp,enemies[i]->atk,enemies[i]->def);
            }
        }
        //Returns -1 if there are no enemies alive
        if(num_alive == 0) return -1;
        //asks for user input
        printText("\nChoose option: ",BASE_SPEED);
        opt = 0;
        if(opt < 1 || opt > num_enemies){
            scanf("%d", &opt);
            if(0 < opt < num_enemies && enemies[opt -1]->hp > 0) return opt-1;
            opt = 0;
            printText("\nChoose a valid option.",BASE_SPEED);
        }
        
    }
    //Returns the targetted enemy
    return opt-1;
}

//Function to handle the turn of the player
void playerTurn(Turn *turn, Enemy *enemies[MAX_ENEMIES], Character *character, int num_enemies, int *active, Time_Strike *time_strk){
    clearScreen();
    if(enemies != NULL){
        printText("Player Turn:\n",BASE_SPEED);
        Enemy *target; 
        //Obtains the target
        int chosen_target_index = selectTarget(enemies,num_enemies);
        if(chosen_target_index != -1){
            target = enemies[chosen_target_index];
            Skills *skill;
            //obtains the skill to use
            skill = selectSkill(character,time_strk);

            //Add skill to time strike array
            time_strk->array[time_strk->num_skills] = *skill;
            time_strk->num_skills++;

            //Handles the skill modifiers
            Turn *current_turn = turn;
            for(int i = 0;i < skill->duration;i++){
                //Atk Modifier
                current_turn->mod[0] += skill->modifiers[0];
                //Def Modifier
                current_turn->mod[1] += skill->modifiers[1];
                //Hp Modifier
                current_turn->mod[2] += skill->modifiers[2];
                if(current_turn->next != NULL) current_turn = current_turn->next;
            }
            current_turn = turn;
            //Prints the current modifiers
            printFormattedText("\nModifiers this turn ATK:%d,DEF:%d,HP:%d)",BASE_SPEED,current_turn->mod[0],current_turn->mod[1],current_turn->mod[2]);
            
            //Handling of the damage calculations
            int damage = 0;
            if(target->def > 0) damage = (((character->atk + turn->mod[0]) * skill->damage) - target->def);
            else damage = (character->atk * skill->damage);
            if(damage > target->hp) damage = target->hp;
            if(damage < 0) damage = 0;
            target->hp = target->hp - damage;
            if(target->hp< 0)   target->hp=0;
            printFormattedText("\nYou deal %d damage to enemy, %s has %d health", BASE_SPEED,damage, target->name, target->hp);
            sleep(1);
        }
        //If there are no more enemies
        else{
            printText("\nNo enemies alive, you have defeated every enemy on this area.",BASE_SPEED);
            sleep(2);
            clearScreen();
            *active = 0;
        } 
   }
}

//Function for the enemy to select a random skill to use
void selectEnemySkill(Enemy *current_enemy, Skills *skill){
    //Select skill randomly+
    int random = rand()%current_enemy->num_skills;
    *skill = current_enemy->skill[random];
}

//Function to handle the turn of the enemy
Queue* enemyTurn(Queue *queue, Character *character, int *active){
    sleep(1);
    clearScreen();
    int hp = character->current_hp;
    Enemy *current_enemy = queue->first->enemy;
    //If the enemy acting on this current turn is dead, return so that it doesn't do anything
    if(current_enemy->hp <= 0){ 
        return queue;
    }
    printText("Enemy Turn:\n",BASE_SPEED);
    printFormattedText("\n-%s attacks you!!!",BASE_SPEED,queue->first->name);
    //Selecting enemy skill
    Skills *skill = malloc(MAX_SKILLS*sizeof(Skills));
    selectEnemySkill(current_enemy,skill);
    printFormattedText("\n-%s uses the skill %s",BASE_SPEED,queue->first->name,skill->name);

    //Handles modifiers
    Turn *current_turn = queue->first;
    for(int i = 0;i < skill->duration;i++){
        //Atk Mod
        current_turn->mod[0] += skill->modifiers[0];
        //Def Mod
        current_turn->mod[1] += skill->modifiers[1];
        //Hp Mod
        current_turn->mod[2] += skill->modifiers[2];
        if(current_turn->next != NULL) current_turn = current_turn->next;
    }
    current_turn = queue->first;
    printFormattedText("\nModifiers this turn ATK:%d,DEF:%d,HP:%d)",BASE_SPEED,current_turn->mod[0],current_turn->mod[1],current_turn->mod[2]);
    sleep(2);

    //Handles damage calculations 
    int damage = 0;
    if(character->def > 0) damage = ((current_enemy->atk * skill->damage) - (character->def + current_turn->mod[1]))/10;
    else damage = (current_enemy->atk * skill->damage)/10;
    if(damage < 0) damage = 0;
    character->current_hp = hp + current_turn->mod[2] - damage;
    if(character->current_hp < 0)   character->current_hp=0;
    printFormattedText("\n-deals %d damage, remaining health --> %d",BASE_SPEED,damage,character->current_hp);
    sleep(1);
    //Checks if player is dead
    if (character->current_hp <= 0){
        //End battle if its dead
        *active = 0;
        return queue;
    }
    //continue battle if character is still alive
    *active = 1;
    return queue;
}


//Function to handle new stat allocation after each successful battle
//Same functionality as the configure_stats function but adapted to work after a battle
void add_stats(Character *character,int num){
    clearScreen();
    int input;
    int input2;
    int current_points = num;
    int base_atk = character->atk;
    int base_def = character->def;;
    int base_hp = character->hp;;
    printFormattedText("\nYou have obtained %d points to upgrade your stats",BASE_SPEED,num);
    sleep(3);
    
    while(current_points > 0){
        clearScreen();
            printFormattedText("\n\n%s's current stats: Atk:%d        Def:%d      Hp:%d\n",BASE_SPEED,character->name,character->atk,character->def,character->hp);
            printFormattedText("\nAvailable SP(Stat Points):%d SP\n",BASE_SPEED,current_points);
            printFormattedText("\nInvested SP:    1.Atk:%d      2.Def:%d      3.HP:%d\n",BASE_SPEED, character->atk-base_atk,character->def-base_def,(character->hp-base_hp) / HP_PER_POINT);
            printText("\nSelect which stat you want to invest in: ",BASE_SPEED);
            scanf("%d", &input);
            if(input < 1 || input>3){
                printText("\nNot a valid element, try again.",BASE_SPEED);
                sleep(1);
                continue;
            }
            else{
                while(1){
                    clearScreen();
                    printText("\nHow many stat points do you want to invest in this stat?(Press 0 to not invest in this stat for now): ",BASE_SPEED);
                    scanf("%d", &input2);
                    if(input2 > current_points || input2 < 0){
                        printText("\nNot enough stat points available.",BASE_SPEED);
                        sleep(1);
                        continue;
                    }
                    else if(input2 > 0){
                        if(input == 1){
                            character->atk += input2;
                            current_points -= input2;
                            break;
                        }
                        else if(input == 2){
                            character->def += input2;
                            current_points -= input2;
                            break;
                        }
                        else if(input == 3){
                            character->hp += input2 * HP_PER_POINT;
                            current_points -= input2;
                            break;
                        }
                    }
                }
                
            }
            if(current_points == 0){
                while(1){
                    clearScreen();
                    while (getchar() != '\n');
                    int input3;

                    printFormattedText("\nFinal stat allocation:    Atk:%d        Def:%d      Hp:%d\n",BASE_SPEED,character->atk,character->def,character->hp);
                    printText("\nConfirm stat allocation?  1.Yes   2.No: ",BASE_SPEED);
                    scanf("%d",&input3);

                    if(input3 == 1){
                        return;
                    }
                    else if(input3 == 2){
                        character->atk = 5;
                        character->def = 5;
                        character->hp = 10;
                        current_points = 20;
                        break;
                    }
                    else{
                        current_points = 0;
                    }
                }
            }
        } 
}


//Handling of turn based combat 
int combat(Character *character, Enemy *enemies[MAX_ENEMIES]){
    printf("\nCOMBAT:\n");
    int active = 1;
    int num_enemies = 0;

    //Gets the enemy data
    for (int i = 0; i<MAX_ENEMIES;i++){
        if(enemies[i] != NULL){
            get_enemy_data(enemies[i]);
            num_enemies++;
        }
    }

    //Create Time Strike struct
    Time_Strike *time_strk = malloc(sizeof(Time_Strike));
    Skills *skills = malloc(MAX_SKILLS * sizeof(Skills));
    time_strk->array = skills;
    time_strk->num_skills = 0;
    time_strk->used = 0;
    
    //creates the queue based turn order
    Queue *queue = malloc(sizeof(Queue));
    queue = createQueue(character,enemies,queue,num_enemies);
    //Handles turn order
    while(active && queue->first != NULL){
        clearScreen();
        if(queue->first->type == 0){
            //When it's player's turn
            playerTurn(queue->first, enemies, character,num_enemies,&active,time_strk);
            sleep(1);
        }
        else{
            //When it's enemy's turn
            queue = enemyTurn(queue, character,&active);
            
        } 
        //Handles player death
        if(character->current_hp <= 0  || queue->first->next == NULL) return 0;
        queue->first = queue->first->next;
    }
    //After victory, 20 new stat points to add to character
    free(time_strk);
    free(queue);
    add_stats(character,20);
    return 1;
}
#endif