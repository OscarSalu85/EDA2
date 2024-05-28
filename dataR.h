
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h" // Include the cJSON library header
#include <unistd.h>
#define SKILL_MAX 15 //NUMBER OF SKILLS IN THE GAME THAT THE PLAYER CAN OBTAIN
#define ENEMY_SKILL_MAX 32
#ifndef DATAR_H
#define DATAR_H
//OPENS JSON IN READ MODE
cJSON startup_read(char json[]){
   FILE *fp = fopen(json, "r"); 
   cJSON cj;
    if (fp == NULL) { 
        //Code to handle possible errors when opening the file
        perror("getcwd() error");
        printf("Error: Unable to open the file.\n"); 
        return cj; 
    } 
  
    // Convert the file into a string data
    char buffer[12072]; 
    int len = fread(buffer, 1, sizeof(buffer), fp); 
    fclose(fp); 
    
    // Parse the JSON data 
    cJSON *root = cJSON_Parse(buffer); 
    if (root == NULL) { 
        const char *error_ptr = cJSON_GetErrorPtr(); 
        if (error_ptr != NULL) { 
            printf("Error: %s\n", error_ptr); 
        } 
        cJSON_Delete(root); 
        free(root);
        return cj; 
    }
    return *root;
}


//Function to save or overwrite the character data into a json file
void save_data(Data *data, int mode){
    //Calls the open file on read function
    cJSON root = startup_read("SaveData.json");
    //Gets the necessary items in the json hierarchy
    cJSON *character = cJSON_GetObjectItem(&root, "Character");
    cJSON *stats = cJSON_GetObjectItem(character, "stats");
    cJSON *skills = cJSON_GetObjectItem(character, "skills");
    cJSON *scenario = cJSON_GetObjectItem(&root, "Current_Scenario");
    //Reset stats,name and scenario in file to original values when mode 0 is passed as parameter
    if(mode == 0){
        cJSON_ReplaceItemInObject(character, "name", cJSON_CreateString(""));
        cJSON_ReplaceItemInObject(stats, "HP", cJSON_CreateNumber(0));
        cJSON_ReplaceItemInObject(stats, "ATK", cJSON_CreateNumber(0));
        cJSON_ReplaceItemInObject(stats, "DEF", cJSON_CreateNumber(0));
        //Reset skills data
        for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
            //Iterates through the skills array
            cJSON *skill = cJSON_GetArrayItem(skills, i);
            cJSON_ReplaceItemInObject(skill, "skill_name", cJSON_CreateString(""));
        }
        //Reset scenario saved data
        cJSON_ReplaceItemInObject(scenario,"name",cJSON_CreateString(""));
    }

    //Saves the character data if mode 1 is passed as parameter
    else if(mode == 1){
        cJSON_ReplaceItemInObject(character, "name", cJSON_CreateString(data->character->name));
        cJSON_ReplaceItemInObject(stats, "HP", cJSON_CreateNumber(data->character->hp));
        cJSON_ReplaceItemInObject(stats, "ATK", cJSON_CreateNumber(data->character->atk));
        cJSON_ReplaceItemInObject(stats, "DEF", cJSON_CreateNumber(data->character->def));

        //Replace skills with current character's skills
        char *skill_name;
        for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
            //Iterates through the skills array
            cJSON *skill = cJSON_GetArrayItem(skills, i);
            skill_name = data->character->skill[i]->name;
            cJSON_ReplaceItemInObject(skill, "skill_name", cJSON_CreateString(skill_name));
        }
        cJSON_ReplaceItemInObject(scenario,"name",cJSON_CreateString(data->current_scenario->name));
        

    }
    //Opens the savedata file in write mode and applies the changes.
    FILE *file = fopen("SaveData.json", "w");
    if (file) {
        char *json_string = cJSON_Print(&root);
        fputs(json_string, file);
        fclose(file);
        free(json_string);
    }
}

//Function to load the character's data from the json file
void load_data(Data *data){
    //Calls the open file on read function
    cJSON root = startup_read("SaveData.json");
    //Gets the necessary items in the json hierarchy
    cJSON *character = cJSON_GetObjectItem(&root, "Character");
    cJSON *stats = cJSON_GetObjectItem(character, "stats");
    cJSON *skills = cJSON_GetObjectItem(character, "skills");
    cJSON *scenario = cJSON_GetObjectItem(&root, "Current_Scenario");

    //Obtains the json objects holding the data we want
    cJSON *scenario_name = cJSON_GetObjectItem(scenario, "name");
    cJSON *name = cJSON_GetObjectItem(character, "name");
    cJSON *atk = cJSON_GetObjectItem(stats, "ATK");
    cJSON *def = cJSON_GetObjectItem(stats, "DEF");
    cJSON *hp = cJSON_GetObjectItem(stats, "HP");

    //Obtains the character data and loads it into the main data structure
    data->character->name = name->valuestring;
    data->character->atk = cJSON_GetNumberValue(atk);
    data->character->def = cJSON_GetNumberValue(def);
    data->character->hp = cJSON_GetNumberValue(hp);
    
    //Obtains all saved skills and loads them into the main data structure
    for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
        //Iterates through the skills array
        cJSON *skill_index = cJSON_GetArrayItem(skills, i);
        cJSON *skill_current = cJSON_GetObjectItem(skill_index, "skill_name");
        data->character->skill[i]->name = skill_current->valuestring;
    }
    
    //Obtains the scenario in which the data was saved and loads it into the main data structure
    data->current_scenario->name = scenario_name->valuestring;
}

//Loads the specific skill information from the name of the skill 
void load_Skill(Skills *skill){
    //Opens the skilldata json file
    cJSON root = startup_read("skillData.json");
    //Gets the necessary items in the json hierarchy
    cJSON *skills = cJSON_GetObjectItem(&root, "skills");
    for (int i = 0; i < cJSON_GetArraySize(skills); i++){
        cJSON *skill_index = cJSON_GetArrayItem(skills, i);
        cJSON *skill_name = cJSON_GetObjectItem(skill_index, "name");
        char *data_skill_name =skill_name->valuestring;
       
        //Checks if the current skill iteration is the desired skill 
        if(strcmp(data_skill_name, skill->name)== 0) {
            //Obtains and transfers the data to the skill data structure
            cJSON *skill_description = cJSON_GetObjectItem(skill_index, "description");
            cJSON *skill_duration = cJSON_GetObjectItem(skill_index, "duration");
            cJSON *skill_damage = cJSON_GetObjectItem(skill_index,"damage");
            cJSON *skill_mods = cJSON_GetObjectItem(skill_index, "modifiers");
            cJSON *mod_1 = cJSON_GetArrayItem(skill_mods, 0);
            cJSON *mod_2 = cJSON_GetArrayItem(skill_mods, 1);
            cJSON *mod_3 = cJSON_GetArrayItem(skill_mods, 2);
            skill->description = skill_description->valuestring;
            skill->duration = cJSON_GetNumberValue(skill_duration);
            skill->damage = cJSON_GetNumberValue(skill_damage);
            skill->modifiers[0] = cJSON_GetNumberValue(mod_1);
            skill->modifiers[1] = cJSON_GetNumberValue(mod_2);
            skill->modifiers[2] = cJSON_GetNumberValue(mod_3);
            //Ends loop when the skill has been found
            break;
        }
    }
}

//Obtains the list of available skills from the json file
Skills* get_skill_data(){
    Skills *skills_array;
    // Allocate memory for n Skills structures
    skills_array = (Skills *)malloc(SKILL_MAX * sizeof(Skills));
    if (skills_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return skills_array;
    }
    //Calls the open file on read function
    cJSON root = startup_read("skillData.json");
    
    cJSON *skills = cJSON_GetObjectItem(&root, "skills");

    //Iterates through every skill and obtains its elements
    for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
        cJSON *skill_index = cJSON_GetArrayItem(skills, i);
        cJSON *skill_name = cJSON_GetObjectItem(skill_index, "name");
        cJSON *skill_description = cJSON_GetObjectItem(skill_index, "description");
        cJSON *skill_duration = cJSON_GetObjectItem(skill_index, "duration");
        cJSON *skill_damage = cJSON_GetObjectItem(skill_index,"damage");
        cJSON *skill_mods = cJSON_GetObjectItem(skill_index, "modifiers");
        cJSON *mod_1 = cJSON_GetArrayItem(skill_mods, 0);
        cJSON *mod_2 = cJSON_GetArrayItem(skill_mods, 1);
        cJSON *mod_3 = cJSON_GetArrayItem(skill_mods, 2);
        
        //Adds the read skill data into the skill array
        skills_array[i].name = skill_name->valuestring;
        
        skills_array[i].description = skill_description->valuestring;

        skills_array[i].duration = skill_duration->valueint;
        
        skills_array[i].damage = cJSON_GetNumberValue(skill_damage);
        skills_array[i].modifiers[0] = cJSON_GetNumberValue(mod_1);
        skills_array[i].modifiers[1] = cJSON_GetNumberValue(mod_2);
        skills_array[i].modifiers[2] = cJSON_GetNumberValue(mod_3);

    }
    return skills_array;
}

//Function to obtain the skills from the enemies
Skills* get_enemy_skill_data(){
    Skills *skills_array;

    // Allocate memory for n Skills structures
    skills_array = (Skills *)malloc(ENEMY_SKILL_MAX * sizeof(Skills));
    if (skills_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return skills_array;
    }
    //Calls the open file on read function
    cJSON root = startup_read("enemySkill.json");
    
    cJSON *skills = cJSON_GetObjectItem(&root, "skills");
    //Iterates through the skills array
    for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
        
        cJSON *skill_index = cJSON_GetArrayItem(skills, i);
        cJSON *skill_name = cJSON_GetObjectItem(skill_index, "name");
        cJSON *skill_description = cJSON_GetObjectItem(skill_index, "description");
        cJSON *skill_duration = cJSON_GetObjectItem(skill_index, "duration");
        cJSON *skill_damage = cJSON_GetObjectItem(skill_index,"damage");
        cJSON *skill_mods = cJSON_GetObjectItem(skill_index, "modifiers");
        cJSON *mod_1 = cJSON_GetArrayItem(skill_mods, 0);
        cJSON *mod_2 = cJSON_GetArrayItem(skill_mods, 1);
        cJSON *mod_3 = cJSON_GetArrayItem(skill_mods, 2);
        
        //Adds the obtained enemy skill data to the skill array
        skills_array[i].name = skill_name->valuestring;
        
        skills_array[i].description = skill_description->valuestring;

        
        skills_array[i].duration = skill_duration->valueint;
        
        skills_array[i].damage = cJSON_GetNumberValue(skill_damage);
        skills_array[i].modifiers[0] = cJSON_GetNumberValue(mod_1);
        skills_array[i].modifiers[1] = cJSON_GetNumberValue(mod_2);
        skills_array[i].modifiers[2] = cJSON_GetNumberValue(mod_3);

    }
    return skills_array;
}

//Function to obtain the overall enemy data from the enemyData json file
void get_enemy_data(Enemy *enemy){
    //Allocates memory for the skill array
    Skills *skills_array;
    skills_array = (Skills *)malloc(SKILL_MAX * sizeof(Skills));

    //Calls the open file on read function
    cJSON root = startup_read("enemyData.json");
    cJSON *enemies = cJSON_GetObjectItem(&root, "Enemies");
    //Looks for the specific enemy in the json file
    cJSON *enemy_json = cJSON_GetObjectItem(enemies, enemy->name);
    
    //Obtains and adds the enemy stats from the json to the enemy
    cJSON *en_def = cJSON_GetObjectItem(enemy_json, "def");
    enemy->def = cJSON_GetNumberValue(en_def);

    cJSON *en_hp = cJSON_GetObjectItem(enemy_json, "hp");
    enemy->hp = cJSON_GetNumberValue(en_hp);

    cJSON *en_atk= cJSON_GetObjectItem(enemy_json, "atk");
    enemy->atk = cJSON_GetNumberValue(en_atk);

    cJSON *en_num_sk= cJSON_GetObjectItem(enemy_json, "num_skills");
    enemy->num_skills = cJSON_GetNumberValue(en_num_sk);

    //Obtains the enemy's skills by calling the get_enemy_skill_data function
    cJSON *en_skills = cJSON_GetObjectItem(enemy_json, "skills");
    Skills *skill_list = get_enemy_skill_data();
    for(int i = 0; i < cJSON_GetArraySize(en_skills);i++){
        cJSON *current_skill = cJSON_GetArrayItem(en_skills, i);
        cJSON *current_skill_name = cJSON_GetObjectItem(current_skill, "name");
        char *name = current_skill_name->valuestring;
        Skills *skill = get_enemy_skill_data();
        for(int j = 0; j<ENEMY_SKILL_MAX ;j++){
            if(strcmp(name, skill[j].name) == 0){
                enemy->skill[i] = skill[j];
            }
        }
    }
}

//Function to obtain and create a list of all the scenarios from the json file
Scenario* get_scenario_nodes(Scenario *scenario_list){
    //Calls the open file on read function
    cJSON root = startup_read("scenarioData.json");
    cJSON *scenario = cJSON_GetObjectItem(&root, "Scenarios");

    //Iterates through the scenarios
    for (int i = 0; i < cJSON_GetArraySize(scenario); i++) {
        //Obtains and adds the relevant information to the scenario list
        cJSON *array_index = cJSON_GetArrayItem(scenario, i);
        cJSON *scenario_name = cJSON_GetObjectItem(array_index, "name");
        cJSON *scenario_desc = cJSON_GetObjectItem(array_index, "description");
        cJSON *scenario_image_file = cJSON_GetObjectItem(array_index, "image");
        cJSON *scenario_question = cJSON_GetObjectItem(array_index,"question");
        cJSON *options_array = cJSON_GetObjectItem(array_index,"options"); 
        
        //Obtains the options available on each scenario
        for(int j = 0; j<cJSON_GetArraySize(options_array); j++){
            cJSON *optIndex = cJSON_GetArrayItem(options_array, j);
            cJSON *optName = cJSON_GetObjectItem(optIndex, "optName");
            cJSON *optText = cJSON_GetObjectItem(optIndex, "optText");
            cJSON *combat_array = cJSON_GetObjectItem(optIndex, "Combat");
            
            //Obtains the name of the enemies found on each scenario
            for(int x = 0; x<cJSON_GetArraySize(combat_array); x++){
                cJSON *enemy_type_list = cJSON_GetArrayItem(combat_array,x);
                cJSON *enemy_type = cJSON_GetObjectItem(enemy_type_list, "enemyName");
                scenario_list[i].decision->options[j]->enemies[x]->name = enemy_type->valuestring;
            }
            strcpy(scenario_list[i].decision->options[j]->n_text , optName->valuestring);
            strcpy(scenario_list[i].decision->options[j]->r_text ,optText->valuestring);
        }
        
        //Obtains the scenarios linked to the current scenario
        cJSON *next_scenario_list = cJSON_GetObjectItem(array_index,"next_scenarios");
        cJSON *next_A = cJSON_GetObjectItem(next_scenario_list,"option_A");
        cJSON *next_B = cJSON_GetObjectItem(next_scenario_list, "option_B");

        scenario_list[i].name = scenario_name->valuestring;
    
        scenario_list[i].description = scenario_desc->valuestring;

        scenario_list[i].image = scenario_image_file->valuestring;
      
        
        strcpy(scenario_list[i].decision->question ,scenario_question->valuestring);
        scenario_list[i].next_scenario_name_1 = next_A->valuestring;
        scenario_list[i].next_scenario_name_2 = next_B->valuestring;
    }
    //Returns the scenario array
    return scenario_list;
}

//Obtains a specific scenario based on its name
Scenario* get_scenario_node(Scenario *scene, char* child_name){
    //Calls the open file on read function
    cJSON root = startup_read("scenarioData.json");
    cJSON *scenario = cJSON_GetObjectItem(&root, "Scenarios");
    for (int i = 0; i < cJSON_GetArraySize(scenario); i++) {
        //Iterates through the scenarios array
        cJSON *array_index = cJSON_GetArrayItem(scenario, i);
        cJSON *scenario_name = cJSON_GetObjectItem(array_index, "name");
        char* scene_name = scenario_name->valuestring;
     
        //Looks for the scenario with the same name as the passed argument 
        if(strcmp(scene_name, child_name) == 0){
            //If it is found, obtain the data from that scenario 
            cJSON *scenario_desc = cJSON_GetObjectItem(array_index, "description");
            cJSON *scenario_image_file = cJSON_GetObjectItem(array_index, "image");
            cJSON *scenario_question = cJSON_GetObjectItem(array_index,"question");
            cJSON *options_array = cJSON_GetObjectItem(array_index,"options"); 
            
            for(int j = 0; j<cJSON_GetArraySize(options_array); j++){
                cJSON *optIndex = cJSON_GetArrayItem(options_array, j);
                cJSON *optName = cJSON_GetObjectItem(optIndex, "optName");
                cJSON *optText = cJSON_GetObjectItem(optIndex, "optText");
                cJSON *combat_array = cJSON_GetObjectItem(optIndex, "Combat");
                
                for(int x = 0; x<cJSON_GetArraySize(combat_array); x++){
                    cJSON *enemy_type_list = cJSON_GetArrayItem(combat_array,x);
                    cJSON *enemy_type = cJSON_GetObjectItem(enemy_type_list, "enemyName");
                    scene->decision->options[j]->enemies[x]->name = enemy_type->valuestring;

                }
                strcpy(scene->decision->options[j]->n_text ,optName->valuestring);
                strcpy(scene->decision->options[j]->r_text ,optText->valuestring);
            }
            

            cJSON *next_scenario_list = cJSON_GetObjectItem(array_index,"next_scenarios");
            cJSON *next_A = cJSON_GetObjectItem(next_scenario_list,"option_A");
            cJSON *next_B = cJSON_GetObjectItem(next_scenario_list, "option_B");

            scene->name = scenario_name->valuestring;

            scene->description = scenario_desc->valuestring;

            scene->image = scenario_image_file->valuestring;


            strcpy(scene->decision->question ,scenario_question->valuestring);
            scene->next_scenario_name_1 = next_A->valuestring;
            scene->next_scenario_name_2 = next_B->valuestring;
            //Returns the found scenario data once it's been extracted
            return scene;   
        }
    }
    //Returns NULL if no matching scenario has been found
    return NULL;
}

#endif