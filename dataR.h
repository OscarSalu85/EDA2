#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.c" // Include the cJSON library header


//OPENS JSON IN READ MODE
cJSON startup_read(char json[]){
   FILE *fp = fopen(json, "r"); 
   cJSON cj;
    if (fp == NULL) { 
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
        return cj; 
    } 
    return *root;
}


void save_character_data(Data *data){
    //Calls the open file on read function
    cJSON root = startup_read("SaveData.json");
    //Gets the necessary items in the json hierarchy
    cJSON *character = cJSON_GetObjectItem(&root, "Character");
    cJSON *stats = cJSON_GetObjectItem(character, "stats");
    cJSON *skills = cJSON_GetObjectItem(character, "skills");
    cJSON *scenario = cJSON_GetObjectItem(&root, "Current_Scenario");
    cJson *scenario_name = cJSON_GetObjectItem(scenario, "name");
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
/*
Scenario extractScenario(char scenario[]){
    Scenario scene;
    cJSON root  = startup("generalData.json");
    cJSON *sections = cJSON_GetObjectItem(&root, "Scenarios");
    cJSON *section;
    cJSON_ArrayForEach(section,sections){
        cJSON *name = cJSON_GetObjectItem(section, "name");
        char *name_str = cJSON_Print(name);
        name_str++;
        name_str[strlen(name_str)-1] = 0;
        if(!strcmp(name_str,scenario)){
            strcpy(scene.name,name_str);
            cJSON *image = cJSON_GetObjectItem(section, "image");
            char *image_file = cJSON_Print(image);
            image_file++;
            image_file[strlen(image_file)-1] = 0;
            strcpy(scene.image,image_file);
            cJSON *description = cJSON_GetObjectItem(section, "description");
            char *description_str = cJSON_Print(description);
            description_str++;
            description_str[strlen(description_str)-1] = 0;
            strcpy(scene.description,description_str);
            return scene;
        }
    }
}


*/