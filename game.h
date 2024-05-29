#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "queue.h"
#include "dataR.h"
#include "combat.h"
#include "printPlus.h"
#define BASE_HP 25
#define BASE_DEF 20
#define BASE_ATK 15
#define INITIAL_STATS 20
#define HP_PER_POINT 2 //Hp value given for every stat point invested

//Function to print the main menu interface and handling the options in that menu
int main_menu(Data *data){
    printImage("title.txt");
    char choice;
    char choice2;
    //Loads the character data from the json file if there's any
    load_data(data);
    while(1)
    {   
        //Printing the menu and asking for user input 
        printf("\n1.Continue      2.New Game     3.Configure \n");
        printText("\nSelect your choice:\n ", BASE_SPEED);
        scanf("%c", &choice);

        //If choice == continue, clear the screen and check if there's saved data. If there is, return the corresponding value (1)
        if(choice == '1'){
            clearScreen();
            //Checks if the first letter on the saved character name is an alphanumeric character. 
            //Since the character name selection limits the player to only alphanumeric characters, this must be true if there's a saved data
            if(!isalpha(data->character->name[0])){
                printText("\nNo saved data found, please start a new game",BASE_SPEED);
                sleep(2);
            }
            else{
                printText("\nSaved data found, continuing game...\n",BASE_SPEED);
                sleep(2);
                //Return the corresponding value
                return 1;
            }
        }
        //If choice == new game, asks the player for confirmation and returns the corresponding value
        else if(choice == '2'){
            clearScreen();
            printText("Are you sure you want to start a new game? All save data will be deleted.(1.Yes  2.No): ", BASE_SPEED);
            while (getchar() != '\n');
            scanf("%c", &choice2);
            if(choice2 == '1'){
                printText("\nStarting new game...", BASE_SPEED);
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
                printText("\nNo saved data found, please start a new game\n", BASE_SPEED);
                sleep(2);
            }
            else{
                printText("\nSaved data found, opening character configuration...", BASE_SPEED);
                sleep(2);
                clearScreen();
                return 3;
            }
        }
        //If no valid option, restart the menu
        else{
            clearScreen();
            printText("\nNot a valid option, please choose again.", BASE_SPEED);
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
        printText("Select your new name: ",BASE_SPEED);
        fgets(data->character->name, MAX_CHAR_NAME, stdin);

        //Handles the possibility of spaces inside the name string. A 2 word name would cause issues without this part of the code
        size_t len = strlen(data->character->name);
        if (len > 0 && data->character->name[len - 1] == '\n') {
            data->character->name[len - 1] = '\0';
        }

        //Checks to verify wether the name is valid or not
        if(strcmp(data->character->name, "")){
            if(data->character->name[0] == '\0' && data->character->name[0] == ' ') {
                printText("\nName cannot be empty, please try again.\n",BASE_SPEED);
                sleep(2);
                continue;
            }
            else{
                //Checks if every letter on the name is either a space or a letter from the alphabet
                for(int i=0;i<strlen(data->character->name);i++){
                    if (!isalpha(data->character->name[i]) && data->character->name[i] != ' '){
                        printText("\nThe name can only contain letters from the alphabet, try again.\n",BASE_SPEED);
                        sleep(2);
                        count = 1;
                        break;
                    }
                    
                }
                //If name passes the checks:
                if(count == 0){
                    while(1){
                        //Asks for name confirmation
                        printFormattedText("\nName Selected: %s\nConfirm name? (1 = yes, 2 = no): ",BASE_SPEED, data->character->name);
                        //Checks wether the input was a valid option or not
                        if (scanf("%d", &choice) != 1) {
                            printText("\nNot a valid option, try again.\n",BASE_SPEED);
                            sleep(2);
                            // Clear invalid input from buffer
                            while (getchar() != '\n');
                            continue;
                        }
                        
                        // Clear the newline character left in the buffer
                        while (getchar() != '\n');
                        //If the name is valid, checks the inputed confirmation
                        if (choice == 1) {
                            return;
                        } else if (choice == 2) {
                            break;
                        } else {
                            printText("\nNot a valid option, try again.\n",BASE_SPEED);
                            sleep(2);
                        }
                    } 
                }
            }
        }
    }
}


//Function to configure the character's stats
void configure_stats(Data* data){

    int previous_points = 0;
    previous_points = data->character->atk - BASE_ATK + data->character->def - BASE_DEF + (data->character->hp - BASE_HP)/2;
    data->character->atk = BASE_ATK;
    data->character->def = BASE_DEF;
    data->character->hp = BASE_HP;   //BASE ATK, DEF AND HP VALUES
    int input;
    int input2;
    int current_points = 20;
    if(1000 > previous_points && previous_points > current_points) current_points = previous_points;
    //While loop until there's no more points to spend
    while(current_points > 0){
        clearScreen();
            //Prints the necessary information
            printFormattedText("\n\n%s's current stats: Atk:%d        Def:%d      Hp:%d\n",BASE_SPEED,data->character->name,data->character->atk,data->character->def,data->character->hp);
            printFormattedText("\nAvailable SP(Stat Points):%d SP\n",BASE_SPEED,current_points);
            printFormattedText("\nInvested SP:    1.Atk:%d      2.Def:%d      3.HP:%d\n",BASE_SPEED, data->character->atk-BASE_ATK,data->character->def-BASE_DEF,(data->character->hp-BASE_HP) / HP_PER_POINT);
            printText("\nSelect which stat you want to invest in: ",BASE_SPEED);
            //asks for user input and checks if it's a valid input
            int result = scanf("%d", &input);
            int c;
            while ((c = getchar()) != '\n');
            if(input < 1 || input>3 || result == 0){
                printText("\nNot a valid element, try again.",BASE_SPEED);
                sleep(2);
                continue;
            }
            //If it is a valid input, execute the code below
            else{
                while(1){
                    clearScreen();
                    //Asks for input on stat ammount to spend
                    printText("\nHow many stat points do you want to invest in this stat?(Press 0 to not invest in this stat for now): ",BASE_SPEED);
                    result = scanf("%d", &input2);
                    while ((c = getchar()) != '\n');
                    if(input2 > current_points || input2 < 0){
                        printText("\nNot enough stat points available.",BASE_SPEED);
                        sleep(2);
                        continue;
                    }
                    else if(result == 0){
                        printText("\nNot a valid element, try again.",BASE_SPEED);
                        sleep(2);
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
                    int input3;
                    printFormattedText("\nFinal stat allocation:    Atk:%d        Def:%d      Hp:%d\n",BASE_SPEED,data->character->atk,data->character->def,data->character->hp);
                    printText("\nConfirm stat allocation?  1.Yes   2.No: ",BASE_SPEED);
                    scanf("%d",&input3);
                    while ((c = getchar()) != '\n');
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
                        printText("\nNot a valid element, try again.",BASE_SPEED);
                        sleep(2);
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
        printFormattedText("\n%d. %s",1000, i+1, skill_list[i].name);
    }
}

//Function to select skills at the start of the game
void configure_skills(Data* data){
    int confirm;
    int selected_skill;
    int repeat;
    Skills *skill_list;
    Skills blank_skill;
    //Calls the get_skill_data function to have access to all the available skills
    skill_list = get_skill_data();
    while(1){
        for(int i=0; i<MAX_SKILLS;i++){
            while(1){
                clearScreen();
                print_skill_list(skill_list);
                repeat = 0;
                //Asks the user to select the skill they want to add to their arsenal.
                printFormattedText("\nSelect your skill number %d: ",BASE_SPEED, i+1);
                int c;
                int scan = scanf("%d",&selected_skill);
                while ((c = getchar()) != '\n');
                selected_skill--;
                //Checks wether input is valid or not
                if(selected_skill<0 || selected_skill > SKILL_MAX - 1 || scan == 0){
                    printText("\nNo valid skill was selected, please choose a valid skill.",BASE_SPEED);
                    sleep(3);
                    continue;
                }
                else{
                    for(int j = 0; j<MAX_SKILLS;j++){
                        //Checks if the player has already chosen this skill to avoid a player to have the same skill multiple times
                        if(skill_list[selected_skill].name ==  data->character->skill[j]->name){
                            printText("\nYou have already chosen this skill, select a different one.",BASE_SPEED);
                            sleep(3);
                            repeat = 1;
                            break;
                        }
                    }
                    if(repeat == 1){
                        repeat = 0;
                        continue;
                    }
                    //Prints the skill information so the player can see what the skill they are choosing does
                    printFormattedText("\nSkill Name: %s",BASE_SPEED, skill_list[selected_skill].name);
                    printFormattedText("\n%s",BASE_SPEED, skill_list[selected_skill].description);
                    printFormattedText("\nSkill Duration: %d turns",BASE_SPEED, skill_list[selected_skill].duration);
                    printFormattedText("\nDamage: %d",BASE_SPEED, skill_list[selected_skill].damage);
                    printFormattedText("\nSelf modifiers: (%d ATK, %d DEF, %d HP)",BASE_SPEED, skill_list[selected_skill].modifiers[0],skill_list[selected_skill].modifiers[1],skill_list[selected_skill].modifiers[2]);
                    
                    //Asks for user confirmation
                    printFormattedText("\n\nDo you want to equip this skill as your skill number %d (1.Yes 2.No): ",BASE_SPEED, i+1);
                    int c;
                    int result = scanf("%d", &confirm);
                    while ((c = getchar()) != '\n');
                    if(confirm == 1 && result != 0){
                        //Adds the selected skill to the character's skill structure
                        clearScreen();
                        *data->character->skill[i] = skill_list[selected_skill];
                        printFormattedText("\n%s added to your character's skill set",BASE_SPEED, skill_list[selected_skill].name);
                        sleep(2);
                    }
                    else{
                        continue;
                    }
                    break;
                }
            }
        }
        clearScreen();
        printFormattedText("\n%s's current skill set: ",BASE_SPEED, data->character->name);
        for(int i = 0; i<MAX_SKILLS; i++){
            printFormattedText("\n-%s",BASE_SPEED, data->character->skill[i]->name);
        }
        //Asks for confirmation
        printText("\n\nConfirm skill set?\n1.Yes\n2.No\n",BASE_SPEED );
        int result = scanf("%d", &confirm);
        int c;
        while ((c = getchar()) != '\n');
        while(result == 0){
                printText("\nNot a valid input, try again.",BASE_SPEED);
                sleep(2);
                printText("\n\nConfirm skill set?\n1.Yes\n2.No\n",BASE_SPEED );
                result = scanf("%d", &confirm);
                int c;
                while ((c = getchar()) != '\n');
        } 
        if(confirm == 1 && result != 0){
            clearScreen();
            return;
        }
        for(int i = 0; i<MAX_SKILLS; i++){
            *data->character->skill[i] = blank_skill;
        }
        repeat = 0;
    }
}

//Function to handle the configure option from the main menu
void configure_menu(Data *data){
    int choice_configure;
    while(1){
        clearScreen();
        printText("\n1.Change Name\n2.Change stat allocation\n3.Change skill set\n4.Back to Main Menu",BASE_SPEED);
        printText("\nSelect an option: ",BASE_SPEED);
        int c;
        scanf("%d", &choice_configure);
        while ((c = getchar()) != '\n');
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
            printText("No valid option was chosen, please try again.",BASE_SPEED);
            sleep(1);
            continue;
        }
        //Makes sure to save the new data after each configuration
        save_data(data,1);
    }
    
};


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
    data->current_scenario = &data->sceneNodes[0];

    save_data(data,1); //Saves the configured data
    return;
}

//Function to handle game completion
void isTerminal(Data *data){
    //Asks user input on wether to restart the game or exit
    printText("\nCONGRATULATIONS!!!\nYOU DEFEATED THE MONSTERS AND SAVED THE SHIP\nDo you wish to start a new game? (Yes:1, No:2)",BASE_SPEED);
    char decision;
    int c;
    scanf(" %c", &decision);
    while ((c = getchar()) != '\n');
    if(decision == '1'){
        new_game(data);
    }
    else if(decision == '2'){
        printText("\nEnding game...",BASE_SPEED);
        printText("\nThanks for playing!",BASE_SPEED);
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
    //Converst the next scenarios from empty strings to null for later if statements
    if(strcmp(scene.next_scenario_name_2, "") == 0){
        scene.next_scenario_name_2 = NULL;
    }
    if(strcmp(scene.next_scenario_name_1, "") == 0){
        scene.next_scenario_name_1 = NULL;
    }

    //Checks if there exist adjacent scenarios
    if(scene.next_scenario_name_1 != NULL || scene.next_scenario_name_2 != NULL){
        //Prints the scenario question asking to which scenario to go next
        printf("\n");
        printText(currentDesc->question,BASE_SPEED);
        printf("\n");
        while(1){
            //Asks for user input in order to advance scenarios
            if(scene.next_scenario_name_1 != NULL && scene.next_scenario_name_2 != NULL){
                int c;
                scanf(" %c",&option);
                while ((c = getchar()) != '\n');
                if(option == '1'){
                    num_option = 1;
                    break;
                }
                else if(option == '2'){
                    num_option = 2;
                    break;
                }
                else { //Handles invalid options
                    printText("\nMust be 1 or 2.\n",BASE_SPEED);
                    sleep(1);
                    continue;
                }
            }
            //Asks for user input to go to the last stage. The next_scenario_1 being NULL indicates that it's the last scenario
            else if(scene.next_scenario_name_2 == NULL){
                printText("The only option now is going to Cellar and confronting the evil presence. There is no way back.",BASE_SPEED);
                printText("\n1.Cellar     2.Cellar\n",BASE_SPEED);
                fflush(stdout);
                int c;
                scanf(" %c",&option);
                while ((c = getchar()) != '\n');
                if(option == '1' || option == '2'){
                    num_option = 1;
                    break;
                }
                //Doesn't allow the player to do anything but go to the last scenario
                else{
                    printText("\nYou can't turn back now, you must step down to the cellar.",BASE_SPEED);
                    sleep(1);
                    clearScreen();
                    continue;
                }
            }
        }
        
        data->character->current_hp = data->character->hp; //Heals in between scenarios
        //Auto saves the data after each scenario
        save_data(data,1);
        //Obtains the options available
        Option *currentOpt = currentDesc->options[num_option -1];
        //Starts the combat on the scenario
        clearScreen();
        printFormattedText("%s\n", BASE_SPEED,currentDesc->options[num_option-1]->r_text);
        sleep(5);
        int win = combat(data->character, currentOpt->enemies);
        //If character wins:
        if(win != 0){
            char *next;
            //Goes to the next scenario based on the selected option
            if(num_option == 2){
                next = scene.next_scenario_name_2;
            }
            else if(num_option == 1){
                next = scene.next_scenario_name_1;
            }
            //Obtains the specific data on the next scenario by iterating through the scenario list
            for(int i = 0;i<SCENARIO_N;i++){
                if(!strcmp(next,data->sceneNodes[i].name)){
                    *data->current_scenario = data->sceneNodes[i];
                }
            }
            return 1;
        }
        //Handles whenever a battle was lost
        else{
            printText("\nYou lost the battle, after backing up for a little you returned to the room\n",BASE_SPEED);
            //Restores hp back to the max hp
            data->character->current_hp = data->character->hp;
            //Saves the player data
            save_data(data,1);
            return 0;
        } 
    }
    //Modified code from above to operate when it's the last scenario
    else{
        data->character->current_hp = data->character->hp; //Heals in between scenarios
        save_data(data,1);
        Option *currentOpt = currentDesc->options[0];
        //Calls the final combat
        int win = combat(data->character, currentOpt->enemies);
        if(win == 0){
            printText("\nYou lost the battle, after backing up for a little you returned to the room\n",BASE_SPEED);
            data->character->current_hp= data->character->hp;
            save_data(data,1);
            return 0;
        }
        //Calls the isTerminal function when the player defeats the last scenario
        isTerminal(data);
        return 0;
    }
}

//Game MainLoop
void mainLoop(Data *data){
    //Sets the scene variable
    Scenario *currentScene = data->current_scenario;
    int active = 1;
    while(active){
        //calls the decision function which starts the scenario traversal
        int active = Decision(data,*currentScene);
        currentScene = data->current_scenario;
    }
}

//Continue game function
void continue_game(Data *data){
    //Allocates data for the current scenario
    Scenario *scene = allocate_scenarios(1);
    
    //Obtains the details on the current scenario
    scene = get_scenario_node(scene, data->current_scenario->name);
    data->current_scenario = scene;
    
    //Loads the skills of the character
    Skills skill[MAX_SKILLS];
    for(int i = 0; i<MAX_SKILLS; i++){
        skill[i].name = data->character->skill[i]->name;
        load_Skill(&skill[i]);
        *data->character->skill[i] = skill[i];
    }
   return;
} 












