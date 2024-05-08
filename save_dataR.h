#ifndef SAVEDATAR_H
#define SAVEDATAR_H
#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.c" // Include the cJSON library header


void get_save_data(Data* data){
    Scenario scene;
    cJSON root  = startup_read("generalData.json");
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




#endif