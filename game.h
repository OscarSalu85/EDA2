#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "interface.h"
#include "queue.h"
#include "dataR.h"
#define BASE_HP 25
#define BASE_DEF 20
#define BASE_ATK 15
#define INITIAL_STATS 20
#define HP_PER_POINT 2 //Hp value given for every stat point invested

//Function to print the main menu interface and handling the options in that menu
int main_menu(Data *data){
    char choice;
    char choice2;
    //Loads the character data from the json file if there's any
    load_data(data);
    while(1)
    {   
        //Printing the menu and asking for user input 
        printf("\n1.Continue      2.New Game     3.Configure \n");
        printText("\nSelect your choice:\n ");
        scanf("%c", &choice);

        //If choice == continue, clear the screen and check if there's saved data. If there is, return the corresponding value (1)
        if(choice == '1'){
            clearScreen();
            //Checks if the first letter on the saved character name is an alphanumeric character. 
            //Since the character name selection limits the player to only alphanumeric characters, this must be true if there's a saved data
            if(!isalpha(data->character->name[0])){
                printText("\nNo saved data found, please start a new game");
                sleep(2);
            }
            else{
                printText("\nSaved data found, continuing game...\n");
                sleep(2);
                //Return the corresponding value
                return 1;
            }
        }
        //If choice == new game, asks the player for confirmation and returns the corresponding value
        else if(choice == '2'){
            clearScreen();
            printText("Are you sure you want to start a new game? All save data will be deleted.(1.Yes  2.No)");
            while (getchar() != '\n');
            scanf("%c", &choice2);
            if(choice2 == '1'){
                printText("\nStarting new game...");
                sleep(2);
                clearScreen();
                return 2;
            }
            clearScreen();
        }
        //If choice == configure, clear the screen and check if there's saved data. If there is, return the corresponding value (3)
        else if(choice == '3'){
            clearScreen();
            //Checks if the first letter on the saved character name is an alphanumeric character. 
            //Since the character name selection limits the player to only alphanumeric characters, this must be true if there's a saved data
            if(!isalpha(data->character->name[0])){
                printText("\nNo saved data found, please start a new game\n");
                sleep(2);
            }
            else{
                printText("\nSaved data found, opening character configuration...");
                sleep(2);
                clearScreen();
                return 3;
            }
        }
        //If no valid option, restart the menu
        else{
            clearScreen();
            printText("\nNot a valid option, please choose again.");
            sleep(2);
        }
        //Clearing input buffer to avoid errors
        while (getchar() != '\n');
    //clears the screen for interface cleanliness
    clearScreen();
    }
}

//Function to introduce a character's name
void configure_name(Data* data){
    //Initializes the necessary variables s
    int count = 0;
    int choice = 0;
    while (getchar() != '\n');
    //Allocates data for the character name
    data->character->name = malloc(MAX_CHAR_NAME * sizeof(char));

    //Handles memory allocation errors
    if (data->character->name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    //While loop to repeat the code until a valid option is obtained 
    while (1)
    {
        //Cleans the screen
        clearScreen();
        count = 0;
        //Asks for user input on the new name
        printText("Select your new name: ");
        fgets(data->character->name, MAX_CHAR_NAME, stdin);

        //Handles the possibility of spaces inside the name string. A 2 word name would cause issues without this part of the code
        size_t len = strlen(data->character->name);
        if (len > 0 && data->character->name[len - 1] == '\n') {
            data->character->name[len - 1] = '\0';
        }

        //Checks to verify wether the name is valid or not
        if(strcmp(data->character->name, "")){
            if(data->character->name[0] == '\0' && data->character->name[0] == ' ') {
                printText("\nName cannot be empty, please try again.");
                sleep(1);
                continue;
            }
            else{
                //Checks if every letter on the name is either a space or a letter from the alphabet
                for(int i=0;i<strlen(data->character->name);i++){
                    if (!isalpha(data->character->name[i]) && data->character->name[i] != ' '){
                        printf("\nThe name can only contain letters from the alphabet, try again.\n");
                        count = 1;
                        break;
                    }
                    
                }
                //If name passes the checks:
                if(count == 0){
                    while(1){
                        //Asks for name confirmation
                        printFormattedText("\nName Selected: %s\nConfirm name? (1 = yes, 0 = no): ", data->character->name);
                        //Checks wether the input was a valid option or not
                        if (scanf("%d", &choice) != 1) {
                            printText("\nNot a valid option, try again.\n");
                            sleep(1);
                            // Clear invalid input from buffer
                            while (getchar() != '\n');
                            continue;
                        }
                        
                        // Clear the newline character left in the buffer
                        while (getchar() != '\n');
                        //If the name is valid, checks the inputed confirmation
                        if (choice == 1) {
                            return;
                        } else if (choice == 0) {
                            break;
                        } else {
                            printText("\nNot a valid option, try again.\n");
                            sleep(1);
                        }
                    } 
                }
            }
        }
    }
}


//Function to configure the character's stats
void configure_stats(Data* data){
    data->character->atk = BASE_ATK;
    data->character->def = BASE_DEF;
    data->character->hp = BASE_HP;   //BASE ATK, DEF AND HP VALUES
    int input;
    int input2;
    int current_points = 20;
    //While loop until there's no more points to spend
    while(current_points > 0){
        clearScreen();
            //Prints the necessary information
            printFormattedText("\n\n%s's current stats: Atk:%d        Def:%d      Hp:%d\n",data->character->name,data->character->atk,data->character->def,data->character->hp);
            printFormattedText("\nAvailable SP(Stat Points):%d SP\n",current_points);
            printFormattedText("\nInvested SP:    1.Atk:%d      2.Def:%d      3.HP:%d\n", data->character->atk-BASE_ATK,data->character->def-BASE_DEF,(data->character->hp-BASE_HP) / HP_PER_POINT);
            printText("\nSelect which stat you want to invest in: ");
            //asks for user input and checks if it's a valid input
            scanf("%d", &input);
            if(input < 1 || input>3){
                printText("\nNot a valid element, try again.");
                sleep(1);
                continue;
            }
            //If it is a valid input, execute the code below
            else{
                while(1){
                    clearScreen();
                    //Asks for input on stat ammount to spend
                    printText("\nHow many stat points do you want to invest in this stat?(Press 0 to not invest in this stat for now): ");
                    scanf("%d", &input2);
                    if(input2 > current_points || input2 < 0){
                        printText("\nNot enough stat points available.");
                        sleep(1);
                        continue;
                    }
                    //Applies the stat allocation
                    else if(input2 > 0){
                        if(input == 1){
                            data->character->atk += input2;
                            current_points -= input2;
                            break;
                        }
                        else if(input == 2){
                            data->character->def += input2;
                            current_points -= input2;
                            break;
                        }
                        else if(input == 3){
                            data->character->hp += input2 * HP_PER_POINT;
                            current_points -= input2;
                            break;
                        }
                    }
                }
                
            }
            //Once there's no more points left, asks for final confirmation
            if(current_points == 0){
                while(1){
                    clearScreen();
                    while (getchar() != '\n');
                    int input3;
                    printFormattedText("\nFinal stat allocation:    Atk:%d        Def:%d      Hp:%d\n",data->character->atk,data->character->def,data->character->hp);
                    printText("\nConfirm stat allocation?  1.Yes   2.No: ");
                    scanf("%d",&input3);
                    //Returns if input is yes
                    if(input3 == 1){
                        return;
                    }
                    //Sets the stats back if input is not and restarts the process
                    else if(input3 == 2){
                        data->character->atk = BASE_ATK;
                        data->character->def = BASE_DEF;
                        data->character->hp = BASE_HP;
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

//Prints every available skill the character can select as an initial skill. 
void print_skill_list(Skills *skill_list){
    clearScreen();
    for(int i=0; i<SKILL_MAX; i++){
        printf("\n%d.: %s", i+1, skill_list[i].name);
    }
}

//Function to select skills at the start of the game
void configure_skills(Data* data){
    int confirm;
    int selected_skill;
    int repeat;
    Skills *skill_list;
    //Calls the get_skill_data function to have access to all the available skills
    skill_list = get_skill_data();
    while(1){
        for(int i=0; i<MAX_SKILLS;i++){
            while(1){
                clearScreen();
                print_skill_list(skill_list);
                repeat = 0;
                //Asks the user to select the skill they want to add to their arsenal.
                printFormattedText("\nSelect your skill nº%d: ", i+1);
                scanf("%d",&selected_skill);
                selected_skill--;
                //Checks wether input is valid or not
                if(selected_skill<0 || selected_skill >SKILL_MAX){
                    printText("\nNo valid skill was selected, please choose a valid skill.");
                    sleep(1);
                    continue;
                }
                else{
                    for(int j = 0; j<MAX_SKILLS;j++){
                        //Checks if the player has already chosen this skill to avoid a player to have the same skill multiple times
                        if(skill_list[selected_skill].name ==  data->character->skill[j]->name){
                            printText("\nYou have already chosen this skill, select a different one.");
                            sleep(1);
                            repeat = 1;
                            break;
                        }
                    }
                    if(repeat == 1){
                        repeat = 0;
                        continue;
                    }
                    //Prints the skill information so the player can see what the skill they are choosing does
                    printFormattedText("\nSkill Name: %s", skill_list[selected_skill].name);
                    printFormattedText("\n%s", skill_list[selected_skill].description);
                    printFormattedText("\nSkill Duration: %d turns", skill_list[selected_skill].duration);
                    printFormattedText("\nDamage: %d", skill_list[selected_skill].damage);
                    printFormattedText("\nSelf modifiers: (%d ATK, %d DEF, %d HP)", skill_list[selected_skill].modifiers[0],skill_list[selected_skill].modifiers[1],skill_list[selected_skill].modifiers[2]);
                    
                    //Asks for user confirmation
                    printFormattedText("\n\nDo you want to equip this skill as your skill nº%d (1.Yes 2.No): ", i);
                    scanf("%d", &confirm);
                    if(confirm == 1){
                        //Adds the selected skill to the character's skill structure
                        clearScreen();
                        *data->character->skill[i] = skill_list[selected_skill];
                        printFormattedText("\n%s added to your character's skill set", skill_list[selected_skill].name);
                    }
                    else{
                        continue;
                    }
                    break;
                }
            }
        }
        clearScreen();
        printFormattedText("\n%s's current skill set: ", data->character->name);
        for(int i = 0; i<MAX_SKILLS; i++){
            printFormattedText("\n%s", data->character->skill[i]->name);
        }
        //Asks for confirmation
        printText("\nConfirm skill set?\n1.Yes\n2.No\n" );
        scanf("%d", &confirm);
        if(confirm == 1){
            return;
        }
        repeat = 0;
    }
}

//Function to handle the configure option from the main menu
void configure_menu(Data *data){
    int choice_configure;
    while(1){
        clearScreen();
        printText("\n1.Change Name\n2.Change stat allocation\n3.Change skill set\n4.Back to Main Menu");
        printText("\nSelect an option: ");
        scanf("%d", &choice_configure);
        //Checks if the user input is valid and acts accordingly by calling the respective function
        if(choice_configure == 1){
            configure_name(data);
        }
        else if(choice_configure == 2){
            configure_stats(data);
        }
        else if(choice_configure == 3){
            configure_skills(data);
        }
        else if(choice_configure == 4){
            return;
        }
        else{
            printText("No valid option was chosen, please try again.");
            sleep(1);
            continue;
        }
        //Makes sure to save the new data after each configuration
        save_data(data,1);
    }
    
};

//Function to select skill while in combat
Skills* selectSkill(Character *character){
    int opt;
    while(1){
        //Prints all available skills
        for(int i = 0; i < 4;i++){
            printFormattedText("\n%d.%s:\n",i+1,character->skill[i]->name);
        }
        //Asks for user input
        printText("\nChoose option (press 5 if you want to see the skill information): ");
        scanf("%d", &opt);
        while (getchar() != '\n');
        if(0 < opt && opt < 5) {
            return character->skill[opt-1];
        }
        //Shows the details of every skill on screen
        else if(opt == 5){
            clearScreen();
            for(int i = 0; i < 4;i++){
                printf("\n%d.%s:\n",i+1,character->skill[i]->name);
                printf("   -Desc:%s\n",character->skill[i]->description);
                printf("   -Dur:%d\n",character->skill[i]->duration);
                printf("   -Dam:%d\n",character->skill[i]->damage);
                printf("   -Mod: (%d ATK, %d DEF, %d HP)\n",character->skill[i]->modifiers[0],character->skill[i]->modifiers[1],character->skill[i]->modifiers[2]);
                
            }  

            printText("\nChoose option: ");
            scanf("%d", &opt);
            if(0 < opt && opt < 5) {
                clearScreen();
                //Returns the selected skill
                return character->skill[opt-1];
            }
        }
        else{
            printText("\nChoose a valid option.");
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
                printFormattedText("\n%d.%s(hp:%d,atk:%d,def:%d)",i+1,enemies[i]->name,enemies[i]->hp,enemies[i]->atk,enemies[i]->def);
            }
        }
        //Returns -1 if there are no enemies alive
        if(num_alive == 0) return -1;
        //asks for user input
        printText("\nChoose option: ");
        opt = 0;
        if(opt < 1 || opt > num_enemies){
            scanf("%d", &opt);
            if(0 < opt < num_enemies && enemies[opt -1]->hp > 0) return opt-1;
            opt = 0;
            printText("\nChoose a valid option.");
        }
        
    }
    //Returns the targetted enemy
    return opt-1;
}

//Function to handle the turn of the player
void playerTurn(Turn *turn, Enemy *enemies[MAX_ENEMIES], Character *character, int num_enemies, int *active){
    clearScreen();
    if(enemies != NULL){
        printText("Player Turn:\n");
        Enemy *target; 
        //Obtains the target
        int chosen_target_index = selectTarget(enemies,num_enemies);
        if(chosen_target_index != -1){
            target = enemies[chosen_target_index];
            Skills *skill;
            //obtains the skill to use
            skill = selectSkill(character);
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
            //Prints the current modifiers
            printFormattedText("\nModifiers this turn ATK:%d,DEF:%d,HP:%d)",current_turn->mod[0],current_turn->mod[1],current_turn->mod[2]);
            
            //Handling of the damage calculations
            int damage = 0;
            if(target->def > 0) damage = (((character->atk + turn->mod[0]) * skill->damage) - target->def);
            else damage = (character->atk * skill->damage);
            if(damage > target->hp) damage = target->hp;
            if(damage < 0) damage = 0;
            target->hp = target->hp - damage;
            if(target->hp< 0)   target->hp=0;
            printFormattedText("\nYou deal %d damage to enemy, %s has %d health", damage, target->name, target->hp);
            sleep(1);
        }
        //If there are no more enemies
        else{
            printText("\nNo enemies alive, you have defeated every enemy on this area.");
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
    printText("Enemy Turn:\n");
    printFormattedText("\n-%s attacks you!!!",queue->first->name);
    //Selecting enemy skill
    Skills *skill = malloc(MAX_SKILLS*sizeof(Skills));
    selectEnemySkill(current_enemy,skill);
    printFormattedText("\n-%s uses the skill %s",queue->first->name,skill->name);

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
    printFormattedText("\nModifiers this turn ATK:%d,DEF:%d,HP:%d)",current_turn->mod[0],current_turn->mod[1],current_turn->mod[2]);
    sleep(2);

    //Handles damage calculations 
    int damage = 0;
    if(character->def > 0) damage = ((current_enemy->atk * skill->damage) - (character->def + current_turn->mod[1]))/10;
    else damage = (current_enemy->atk * skill->damage)/10;
    if(damage < 0) damage = 0;
    character->current_hp = hp + current_turn->mod[2] - damage;
    if(character->current_hp < 0)   character->current_hp=0;
    printFormattedText("\n-deals %d damage, remaining health --> %d",damage,character->current_hp);
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
    printFormattedText("\nYou have obtained %d point to upgrade your stats",num);
    sleep(3);
    
    while(current_points > 0){
        clearScreen();
            printFormattedText("\n\n%s's current stats: Atk:%d        Def:%d      Hp:%d\n",character->name,character->atk,character->def,character->hp);
            printFormattedText("\nAvailable SP(Stat Points):%d SP\n",current_points);
            printFormattedText("\nInvested SP:    1.Atk:%d      2.Def:%d      3.HP:%d\n", character->atk-base_atk,character->def-base_def,(character->hp-base_hp) / HP_PER_POINT);
            printText("\nSelect which stat you want to invest in: ");
            scanf("%d", &input);
            if(input < 1 || input>3){
                printText("\nNot a valid element, try again.");
                sleep(1);
                continue;
            }
            else{
                while(1){
                    clearScreen();
                    printText("\nHow many stat points do you want to invest in this stat?(Press 0 to not invest in this stat for now): ");
                    scanf("%d", &input2);
                    if(input2 > current_points || input2 < 0){
                        printText("\nNot enough stat points available.");
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

                    printFormattedText("\nFinal stat allocation:    Atk:%d        Def:%d      Hp:%d\n",character->atk,character->def,character->hp);
                    printText("\nConfirm stat allocation?  1.Yes   2.No: ");
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
    clearScreen();
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
    //creates the queue based turn order
    Queue *queue = malloc(sizeof(Queue));
    queue = createQueue(character,enemies,queue,num_enemies);
    //Handles turn order
    while(active && queue->first != NULL){
        clearScreen();
        if(queue->first->type == 0){
            //When it's player's turn
            playerTurn(queue->first, enemies, character,num_enemies,&active);
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
    printText("\nYOU WIN\n");
    add_stats(character,20);
    return 1;
}


//Function to allocate memory for the main data structure
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

//Function to allocate data for the scenarios
Scenario* allocate_scenarios(int num_scenarios) {
    //Allocates the scenario structure
    Scenario* scenarios = malloc(num_scenarios * sizeof(Scenario));
    if (scenarios == NULL) {
        fprintf(stderr, "Memory allocation failed for scenarios\n");
        exit(1);
    }

    //Iterates through the number of scenarios to allocate
    for (int i = 0; i < num_scenarios; i++) {
        //Allocates the scenario name
        scenarios[i].name = malloc(MAX_CHAR_NAME * sizeof(char));
        if (scenarios[i].name == NULL) {
            fprintf(stderr, "Memory allocation failed for scenario name\n");
            exit(1);
        }

        //allocates the scenario description
        scenarios[i].description = malloc(MAX_CHAR_DESC * sizeof(char));
        if (scenarios[i].description == NULL) {
            fprintf(stderr, "Memory allocation failed for scenario description\n");
            exit(1);
        }

        //Allocates the decision data structure
        Decisions *dec = malloc(sizeof(Decisions));
        scenarios[i].decision = dec;
        if (scenarios[i].decision == NULL) {
            fprintf(stderr, "Memory allocation failed for scenario decision\n");
            exit(1);
        }

        //Allocates all the option structures
        for (int j = 0; j < MAX_OPTIONS; j++) {
            Option *opt = malloc(sizeof(Option));
            scenarios[i].decision->options[j] = opt;
            if (scenarios[i].decision->options[j] == NULL) {
                fprintf(stderr, "Memory allocation failed for decision option\n");
                exit(1);
            }
            //Allocates the enemies in each option
            for (int k = 0; k < MAX_ENEMIES; k++) {
                Enemy *enemy = malloc(sizeof(Enemy));
                scenarios[i].decision->options[j]->enemies[k] = enemy;
                if (scenarios[i].decision->options[j]->enemies[k] == NULL) {
                    fprintf(stderr, "Memory allocation failed for enemy\n");
                    exit(1);
                }
                //Allocates the name of each enemy
                char *name;
                scenarios[i].decision->options[j]->enemies[k]->name = name;
                if (scenarios[i].decision->options[j]->enemies[k]->name == NULL) {
                    fprintf(stderr, "Memory allocation failed for enemy name\n");
                    exit(1);
                }
            }
        }

        //Allocates the name of the scenarios adjacent to the current scenario
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

//Function to create a new game 
void new_game(Data *data){
    if(data != NULL){
        free(data); //Frees saved data
    }
    //Allocates data
    create_data(&data);
    //Allocates the scenarios and obtains their information
    data->sceneNodes = allocate_scenarios(8);
    data->sceneNodes = get_scenario_nodes(data->sceneNodes);
    save_data(data, 0);//Overwrites save file with empty file, second parameter = 0 --> Delete data

    //calls the configuration function to set the character's characteristics
    configure_name(data);
    configure_stats(data);
    configure_skills(data);
    //Sets the current scenario to the first scenario in the game
    data->current_scenario[0] = data->sceneNodes[0];
    save_data(data,1); //Saves the configured data
    return;
}

//Function to handle game completion
void isTerminal(Data *data){
    //Asks user input on wether to restart the game or exit
    printText("\nCONGRATULATIONS!!!\nYOU DEFEATED THE MONSTERS AND SAVED THE SHIP\nDo you wish to start a new game? (Yes:1, No:2)");
    char decision;
    scanf(" %c", &decision);
    if(decision == '1'){
        new_game(data);
    }
    else if(decision == '2'){
        printText("\nEnding game...");
        printText("\nThanks for playing!");
        sleep(3);
        save_data(data, 0);//Overwrites save file with empty file, second parameter = 0 --> Delete data
        exit(1);
    }
}

//Function to handle the main game functions
int Decision(Data *data, Scenario scene){
    //Prints the scenario image
    printScenario(data->current_scenario);
    Decisions *currentDesc = scene.decision;
    char option;
    int num_option;
    if(strcmp(scene.next_scenario_name_2, "") == 0){
        scene.next_scenario_name_2 = NULL;
    }
    if(strcmp(scene.next_scenario_name_1, "") == 0){
        scene.next_scenario_name_1 = NULL;
    }
    if(scene.next_scenario_name_1 != NULL || scene.next_scenario_name_2 != NULL){
        printText(currentDesc->question);
        printf("\n");
        while(1){
            if(scene.next_scenario_name_1 != NULL && scene.next_scenario_name_2 != NULL){
                scanf(" %c",&option);
                if(option == '1'){
                    num_option = 1;
                    break;
                }
                else if(option == '2'){
                    num_option = 2;
                    break;
                }
                else {
                    printText("\nMust be 1 or 2.\n");
                    sleep(1);
                    continue;
                }
            }
            else if(scene.next_scenario_name_1 == NULL){
                printText("The only option now is going to Cellar and confronting the evil presence. There is no way back.");
                printText("\n1.Cellar     2.Cellar\n");
                fflush(stdout);
                scanf(" %c",&option);
                if(option == '1' || option == '2'){
                    num_option = 2;
                    break;
                }
                else{
                    printText("\nYou can't turn back now, you must step down to the cellar.");
                    sleep(1);
                    clearScreen();
                    continue;
                }
            }
        }
        
        data->character->current_hp = data->character->hp; //Heals in between scenarios
        save_data(data,1);
        Option *currentOpt = currentDesc->options[num_option -1];
        int win = combat(data->character, currentOpt->enemies);
        if(win != 0){
            char *next;
            
            if(num_option == 2){
                next = scene.next_scenario_name_2;
            }
            else if(num_option == 1){
                next = scene.next_scenario_name_1;
            }
            for(int i = 0;i<SCENARIO_N;i++){
                if(!strcmp(next,data->sceneNodes[i].name)){
                    *data->current_scenario = data->sceneNodes[i];
                }
            }
            return 1;
        }
        else{
            printText("\nYou lost the battle, after backing up for a little you returned to the room\n");
            data->character->current_hp= data->character->hp;
            save_data(data,1);
            return 0;
        } 
    }
    else{
        data->character->current_hp = data->character->hp; //Heals in between scenarios
        save_data(data,1);
        Option *currentOpt = currentDesc->options[0];
        int win = combat(data->character, currentOpt->enemies);
        if(win == 0){
            printText("\nYou lost the battle, after backing up for a little you returned to the room\n");
            data->character->current_hp= data->character->hp;
            save_data(data,1);
            return 0;
        }
        isTerminal(data);
        return 0;
    }
}

void mainLoop(Data *data){
    Scenario *currentScene = data->current_scenario;
    int active = 1;
    while(active){
        int active = Decision(data,*currentScene);
        currentScene = data->current_scenario;
    }
}

void continue_game(Data *data){
    Scenario *scene = allocate_scenarios(1);
    scene = get_scenario_node(scene, data->current_scenario->name);
    data->current_scenario = scene;
    Skills skill[MAX_SKILLS];
    
    for(int i = 0; i<MAX_SKILLS; i++){
        skill[i].name = data->character->skill[i]->name;
        load_Skill(&skill[i]);
        *data->character->skill[i] = skill[i];
    }
   return;
} 












