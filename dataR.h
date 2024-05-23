#ifndef DATAR_H
#define DATAR_H
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h" // Include the cJSON library header
#include <unistd.h>
#define SKILL_MAX 10 //NUMBER OF SKILLS IN THE GAME THAT THE PLAYER CAN OBTAIN

//OPENS JSON IN READ MODE
cJSON startup_read(char json[]){
   FILE *fp = fopen(json, "r"); 
   cJSON cj;
    if (fp == NULL) { 
        perror("getcwd() error");
        printf("Error: Unable to open the file.\n"); 
        return cj; 
    } 
  
    // read the file contents into a string 
    char buffer[4096]; 
    int len = fread(buffer, 1, sizeof(buffer), fp); 
    fclose(fp); 
    
    // parse the JSON data 
    
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

void save_data(Data *data, int mode){
    //Calls the open file on read function
    cJSON root = startup_read("SaveData.json");
    //Gets the necessary items in the json hierarchy
    cJSON *character = cJSON_GetObjectItem(&root, "Character");
    cJSON *stats = cJSON_GetObjectItem(character, "stats");
    cJSON *skills = cJSON_GetObjectItem(character, "skills");
    cJSON *scenario = cJSON_GetObjectItem(&root, "Current_Scenario");
    //Reset stats,name and scenario in file to original values
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

    else if(mode == 1){
        cJSON_ReplaceItemInObject(character, "name", cJSON_CreateString(data->character->name));
        cJSON_ReplaceItemInObject(stats, "HP", cJSON_CreateNumber(data->character->hp));
        cJSON_ReplaceItemInObject(stats, "ATK", cJSON_CreateNumber(data->character->atk));
        cJSON_ReplaceItemInObject(stats, "DEF", cJSON_CreateNumber(data->character->def));
        //Replace skills with current game's skills
        char *skill_name;
        for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
            //Iterates through the skills array
            cJSON *skill = cJSON_GetArrayItem(skills, i);
            skill_name = data->character->skill[i]->name;
            skill_name ++;
            skill_name[strlen(skill_name)-1] = 0;
            cJSON_ReplaceItemInObject(skill, "skill_name", cJSON_CreateString(skill_name));
        }
        cJSON_ReplaceItemInObject(scenario,"name",cJSON_CreateString(data->current_scenario->name));
        

    }
    FILE *file = fopen("SaveData.json", "w");
    if (file) {
        char *json_string = cJSON_Print(&root);
        fputs(json_string, file);
        fclose(file);
        free(json_string);
    }
}

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
    data->character->name = cJSON_Print(name);
    data->character->name++;
    data->character->name[strlen(data->character->name)-1] = 0;
    data->character->atk = cJSON_GetNumberValue(atk);
    data->character->def = cJSON_GetNumberValue(def);
    data->character->hp = cJSON_GetNumberValue(hp);
    
    //Obtains all saved skills and loads them into the main data structure
    for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
        //Iterates through the skills array
        cJSON *skill_index = cJSON_GetArrayItem(skills, i);
        cJSON *skill_current = cJSON_GetObjectItem(skill_index, "skill_name");
        data->character->skill[i]->name = cJSON_Print(skill_current);
        data->character->skill[i]->name++;
        data->character->skill[i]->name[strlen(data->character->skill[i]->name)-1] = '\0';
    }
    
    //Obtains the scenario in which the data was saved and loads it into the main data structure
    data->current_scenario->name = cJSON_Print(scenario_name);
}

void load_Skill(Skills *skill){
    cJSON root = startup_read("skillData.json");
    cJSON *skills = cJSON_GetObjectItem(&root, "skills");
    for (int i = 0; i < cJSON_GetArraySize(skills); i++){
        cJSON *skill_index = cJSON_GetArrayItem(skills, i);
        cJSON *skill_name = cJSON_GetObjectItem(skill_index, "name");
        printf("\nSKILL = %s",cJSON_Print(skill_name));
        printf("\nSAVED SKILL = %s", skill->name);
        char *data_skill_name = cJSON_Print(skill_name);
        data_skill_name ++;
        data_skill_name[strlen(data_skill_name)-1] = 0;
        if(strcmp(data_skill_name, skill->name)== 0) {
            cJSON *skill_description = cJSON_GetObjectItem(skill_index, "description");
            cJSON *skill_duration = cJSON_GetObjectItem(skill_index, "duration");
            cJSON *skill_damage = cJSON_GetObjectItem(skill_index,"damage");
            cJSON *skill_mods = cJSON_GetObjectItem(skill_index, "modifiers");
            cJSON *mod_1 = cJSON_GetArrayItem(skill_mods, 0);
            cJSON *mod_2 = cJSON_GetArrayItem(skill_mods, 1);
            cJSON *mod_3 = cJSON_GetArrayItem(skill_mods, 2);
            skill->description = cJSON_Print(skill_description);
            skill->duration = cJSON_GetNumberValue(skill_duration);
            skill->damage = cJSON_GetNumberValue(skill_damage);
            skill->modifiers[0] = cJSON_GetNumberValue(mod_1);
            skill->modifiers[1] = cJSON_GetNumberValue(mod_2);
            skill->modifiers[2] = cJSON_GetNumberValue(mod_3);
            break;
        }
    }
}

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

    for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
        //Iterates through the skills array
        cJSON *skill_index = cJSON_GetArrayItem(skills, i);
        cJSON *skill_name = cJSON_GetObjectItem(skill_index, "name");
        cJSON *skill_description = cJSON_GetObjectItem(skill_index, "description");
        cJSON *skill_duration = cJSON_GetObjectItem(skill_index, "duration");
        cJSON *skill_damage = cJSON_GetObjectItem(skill_index,"damage");
        cJSON *skill_mods = cJSON_GetObjectItem(skill_index, "modifiers");
        cJSON *mod_1 = cJSON_GetArrayItem(skill_mods, 0);
        cJSON *mod_2 = cJSON_GetArrayItem(skill_mods, 1);
        cJSON *mod_3 = cJSON_GetArrayItem(skill_mods, 2);
        
        skills_array[i].name = cJSON_Print(skill_name);
        skills_array[i].description = cJSON_Print(skill_description);
        if (skill_duration && cJSON_IsNumber(skill_duration)) {
            skills_array[i].duration = skill_duration->valueint;
        } else {
            skills_array[i].duration = 0; // Default value if not found or not a number
        }
        skills_array[i].damage = cJSON_GetNumberValue(skill_damage);
        skills_array[i].modifiers[0] = cJSON_GetNumberValue(mod_1);
        skills_array[i].modifiers[1] = cJSON_GetNumberValue(mod_2);
        skills_array[i].modifiers[2] = cJSON_GetNumberValue(mod_3);

    }
    return skills_array;
}



Scenario* get_scenario_nodes(Scenario *scenario_list){
    //Calls the open file on read function
    cJSON root = startup_read("scenarioData.json");
    cJSON *scenario = cJSON_GetObjectItem(&root, "Scenarios");
    //allocate_scenario_list(&scenario_list, cJSON_GetArraySize(scenario));
    for (int i = 0; i < cJSON_GetArraySize(scenario); i++) {
        //Iterates through the scenarios array
        cJSON *array_index = cJSON_GetArrayItem(scenario, i);
        cJSON *scenario_name = cJSON_GetObjectItem(array_index, "name");
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
                scenario_list[i].decision->options[j]->enemies[x]->name = cJSON_Print(enemy_type);
                scenario_list[i].decision->options[j]->enemies[x]->name++;
                scenario_list[i].decision->options[j]->enemies[x]->name[strlen(scenario_list[i].decision->options[j]->enemies[x]->name)-1] = 0;

            }
            strcpy(scenario_list[i].decision->options[j]->n_text ,cJSON_Print(optName));
            strcpy(scenario_list[i].decision->options[j]->r_text ,cJSON_Print(optText));
        }
        
        cJSON *next_scenario_list = cJSON_GetObjectItem(array_index,"next_scenarios");
        cJSON *next_A = cJSON_GetObjectItem(next_scenario_list,"option_A");
        cJSON *next_B = cJSON_GetObjectItem(next_scenario_list, "option_B");

        scenario_list[i].name = cJSON_Print(scenario_name);
        scenario_list[i].name++;
        scenario_list[i].name[strlen(scenario_list[i].name)-1] = 0;

        scenario_list[i].description = cJSON_Print(scenario_desc);
        scenario_list[i].description++;
        scenario_list[i].description[strlen(scenario_list[i].description)-1] = 0;

        scenario_list[i].image = cJSON_Print(scenario_image_file);
        scenario_list[i].image++;
        scenario_list[i].image[strlen(scenario_list[i].image)-1] = 0;

        strcpy(scenario_list[i].decision->question ,cJSON_Print(scenario_question));
        scenario_list[i].next_scenario_name_1 = cJSON_Print(next_A);
        scenario_list[i].next_scenario_name_2 = cJSON_Print(next_B);
        //printf("%s",scenario_list[i].name);
    }
    return scenario_list;
}

void get_enemy_data(Enemy *enemy){

    Skills *skills_array;
    // Allocate memory for n Skills structures
    skills_array = (Skills *)malloc(SKILL_MAX * sizeof(Skills));
    if (skills_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return skills_array;
    }
    //Calls the open file on read function
    cJSON root = startup_read("enemyData.json");
    cJSON *enemies = cJSON_GetObjectItem(&root, "Enemies");
    cJSON *enemy_json = cJSON_GetObjectItem(enemies, enemy->name);
    
    cJSON *en_def = cJSON_GetObjectItem(enemy_json, "def");
    enemy->def = cJSON_GetNumberValue(en_def);

    cJSON *en_hp = cJSON_GetObjectItem(enemy_json, "hp");
    enemy->hp = cJSON_GetNumberValue(en_hp);

    cJSON *en_atk= cJSON_GetObjectItem(enemy_json, "atk");
    enemy->atk = cJSON_GetNumberValue(en_atk);
}


#endif

