#ifndef DATAR_H
#define DATAR_H
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h" // Include the cJSON library header
#include <unistd.h>
#define SKILL_MAX 20

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
    char buffer[1024]; 
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


void save_data(Data *data){
    //Calls the open file on read function
    cJSON root = startup_read("SaveData.json");
    //Gets the necessary items in the json hierarchy
    cJSON *character = cJSON_GetObjectItem(&root, "Character");
    cJSON *stats = cJSON_GetObjectItem(character, "stats");
    cJSON *skills = cJSON_GetObjectItem(character, "skills");
    cJSON *scenario = cJSON_GetObjectItem(&root, "Current_Scenario");
    cJSON *scenario_name = cJSON_GetObjectItem(scenario, "name");

    //Replace stats,name and scenario in file with current game's data
    cJSON_ReplaceItemInObject(character, "name", cJSON_CreateString(data->character->name));
    cJSON_ReplaceItemInObject(stats, "HP", cJSON_CreateNumber(data->character->hp));
    cJSON_ReplaceItemInObject(stats, "ATK", cJSON_CreateNumber(data->character->atk));
    cJSON_ReplaceItemInObject(stats, "DEF", cJSON_CreateNumber(data->character->def));

    //Replace skills with current game's skills
    for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
        //Iterates through the skills array
        cJSON *skill = cJSON_GetArrayItem(skills, i);
        cJSON_ReplaceItemInObject(skill, "skill_name", cJSON_CreateString(data->character->skill[i]->name));
    }
    //Replace current scenario in save data
    cJSON_ReplaceItemInObject(scenario_name,"name",cJSON_CreateString(data->current_scenario->name));
    //Write the current data on the json
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
    strcpy(data->character->name, cJSON_Print(name));
    data->character->atk = cJSON_GetNumberValue(atk);
    data->character->def = cJSON_GetNumberValue(def);
    data->character->hp = cJSON_GetNumberValue(hp);
    
    //Obtains all saved skills and loads them into the main data structure
    for (int i = 0; i < cJSON_GetArraySize(skills); i++) {
        //Iterates through the skills array
        cJSON *skill_index = cJSON_GetArrayItem(skills, i);
        cJSON *skill_current = cJSON_GetObjectItem(skill_index, "skill_name");
        strcpy(data->character->skill[i]->name, cJSON_Print(skill_current));
    }
    
    //Obtains the scenario in which the data was saved and loads it into the main data structure
    strcpy(data->current_scenario->name, cJSON_Print(scenario_name));
}

void get_skill_data(Skills *skill[SKILL_MAX]){
    //Calls the open file on read function
    cJSON root = startup_read("generalData.json");
    //Creation of the skill_list data structure
    Skills *skill_list[SKILL_MAX];
    for (int i = 0; i < SKILL_MAX; i++) {
        skill_list[i] = malloc(sizeof(Skills));
        if (skill_list[i] == NULL) {
            // Handle memory allocation failure
            // Free previously allocated memory to avoid issues
            for (int j = 0; j < i; j++) {
                free(skill_list[j]);
            }
        }
    }
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
        

        
        strcpy(skill_list[i]->name, cJSON_Print(skill_name));
        strcpy(skill_list[i]->description, cJSON_Print(skill_description));
        skill_list[i]->duration = cJSON_GetNumberValue(skill_duration);
        skill_list[i]->damage = cJSON_GetNumberValue(skill_damage);
        
        skill_list[i]->modifiers[0] = cJSON_GetNumberValue(mod_1);
        skill_list[i]->modifiers[1] = cJSON_GetNumberValue(mod_2);
        skill_list[i]->modifiers[2] = cJSON_GetNumberValue(mod_3);
    }
}

#endif