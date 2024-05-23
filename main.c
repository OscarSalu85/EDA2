#include <stdio.h>
#include "structures.h"
#include "game.h"
#include "cJSON.c"

typedef struct Node_G{
    Scenario *scene;
    struct Node_G *child_1;
    struct Node_G *child_2;    
}Node_G;

typedef struct Graph{
    Node_G *root;
}Graph;


void add_child_left(Node_G *parent, Scenario *scene) {
    if (parent->child_1 != NULL) {
        fprintf(stderr, "Child 1 already exists\n");
        return;
    }
}

void add_child_right(Node_G *parent, Scenario *scene) {
    if (parent->child_2 != NULL) {
        fprintf(stderr, "Child 2 already exists\n");
        return;
    }
}

Node_G* create_node(Scenario *scene) {
    Node_G *new_scenario_node = (Node_G *)malloc(sizeof(Node_G));
    if (new_scenario_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    new_scenario_node->scene = scene;
    if(new_scenario_node->scene->next_scenario_name_1 != NULL){
        Scenario *scene_child_1 = allocate_scenarios(1);
        scene_child_1 = get_scenario_node(scene_child_1, new_scenario_node->scene->next_scenario_name_1);
        add_child_left(new_scenario_node,scene_child_1);
        new_scenario_node->child_1 = create_node(scene_child_1);
    }
    else{
        new_scenario_node->child_1 = NULL;
    }
    if(new_scenario_node->scene->next_scenario_name_2 != NULL){
        Scenario *scene_child_2 = allocate_scenarios(1);
        scene_child_2 = get_scenario_node(scene_child_2, new_scenario_node->scene->next_scenario_name_2);
        add_child_right(new_scenario_node,scene_child_2);
        new_scenario_node->child_2 = create_node(scene_child_2);
    }
    else{
        new_scenario_node->child_2 = NULL;
    }
    return new_scenario_node;
}


Graph* create_graph(Data *data){
    Graph *tree = (Graph *)malloc(sizeof(Graph));
    if (tree == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    tree->root->scene = data->current_scenario;
    return tree;
}


int main(){
    Data *data;
    create_data(&data);
    data->sceneNodes = allocate_scenarios(2);
    data->sceneNodes = get_scenario_nodes(data->sceneNodes);
    int main_menu_choice;
    char* name = "Deck";
    printf("\nAAAAAA");
    data->current_scenario = get_scenario_node(data->current_scenario,name);
    printf("\nBBBBBBB");
    Graph *tree = create_graph(data);
    printf("\nROOT NAME = %s",tree->root->scene->name);
    printf("\nLEFT CHILD = %s", tree->root->child_1->scene->name);
    while(1){
        //Menu inicial
        main_menu_choice = main_menu(data);   
        if(main_menu_choice == 1){
            continue_game(data);
        }
        else if(main_menu_choice == 2){
            new_game(data);
        }
        else if(main_menu_choice == 3){
            configure_menu(data);
        }
        
    }
}
