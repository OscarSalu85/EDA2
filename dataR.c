#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.c" // Include the cJSON library header

int main() {
    // File pointer for reading JSON file
    FILE *file = fopen("generalData.json", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
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
        return 1;
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
        return 1;
    }

    // Get the "skills" array
    cJSON *skills = cJSON_GetObjectItem(root, "skills");
    if (skills == NULL || !cJSON_IsArray(skills)) {
        printf("Error: 'skills' array not found or not an array.\n");
        cJSON_Delete(root);
        free(json_data);
        return 1;
    }

    // Loop through the array to find "name" of "skill_1"
    cJSON *skill;
    cJSON_ArrayForEach(skill, skills) {
        cJSON *name = cJSON_GetObjectItem(skill, "name");
        if (name != NULL && cJSON_IsString(name) && strcmp(name->valuestring, "skill_1") == 0) {
            printf("Found skill_1! Name: %s\n", name->valuestring);
            //break; // Stop searching once skill_1 is found
        }
        
    }
    cJSON_ArrayForEach(skill,skills){
        cJSON *duration = cJSON_GetObjectItem(skill, "duration");
            if (duration != NULL && cJSON_IsString(duration) ) {
                printf("Found skill_1! Duration: %s\n", duration->valuestring);
                break; // Stop searching once skill_1 is found
            }
    }
    // Clean up
    cJSON_Delete(root);
    free(json_data);
    return 0;
}
