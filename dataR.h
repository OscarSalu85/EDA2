#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.c" // Include the cJSON library header

cJSON startup(char json[]){
    FILE *file = fopen(json, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
    }

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for the JSON data
    char *json_data = (char *)malloc(file_size + 1);
    if (json_data == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
    }

    // Read JSON data from file
    size_t bytes_read = fread(json_data, 1, file_size, file);
    json_data[bytes_read] = '\0'; // Null-terminate the string
    fclose(file);

    // Parse JSON data
    cJSON *root = cJSON_Parse(json_data);
    if (root == NULL) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(json_data);
    }
    return *root;
}
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


