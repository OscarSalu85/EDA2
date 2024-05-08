#ifndef OPENJSON_H
#define OPENJSON_H
cJSON startup_read(char json[]){
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

cJSON startup_write(char json[]){
    FILE *file = fopen(json, "w");
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

    // Parse JSON data
    cJSON *root = cJSON_Parse(json_data);
    if (root == NULL) {
        printf("Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        free(json_data);
    }
    return *root;
}
#endif