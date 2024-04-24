#include "structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "printPlus.h"
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
void extractScenario(char scenario[]){
    cJSON root  = startup("generalData.json");
    cJSON *sections = cJSON_GetObjectItem(&root, "Scenarios");
    cJSON *section;
    cJSON_ArrayForEach(section,sections){
        cJSON *name = cJSON_GetObjectItem(section, "name");
        char *string1 = cJSON_Print(name);
        string1++;
        string1[strlen(string1)-1] = 0;
        if(!strcmp(string1,scenario)){
            cJSON *image = cJSON_GetObjectItem(section, "image");
            char *string2 = cJSON_Print(image);
            string2++;
            string2[strlen(string2)-1] = 0;
            printImage(string2);
        }
    }
}

int main() {
   extractScenario("Deck");
}

