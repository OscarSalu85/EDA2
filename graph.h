#include "queque.h"
#include <stdio.h>
#include <stdlib.h>
#include "dataR.h"
#ifndef GRAPH_H
#define GRAPH_H
/*
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
        return;
    }

    tree->root->scene = data->current_scenario;
    return tree;
}

*/
#endif





