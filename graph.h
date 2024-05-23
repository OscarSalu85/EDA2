#ifndef GRAPH_H
#define GRAPH_H
#include "queque.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Node_G{
    Scenario *scene;
    struct Node_G *child_1;
    struct Node_G *child_2;    
}Node_G;

typedef struct Graph{
    Node_G *root;
}Graph;

Node_G* create_node(Scenario *scene) {
    Node_G *new_scenario_node = (Node_G *)malloc(sizeof(Node_G));
    if (new_scenario_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    new_scenario_node->scene = scene;
    new_scenario_node->child_1 = NULL;
    new_scenario_node->child_2 = NULL;
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

void add_child_1(Node_G *parent, Scenario *scene) {
    if (parent->child_1 != NULL) {
        fprintf(stderr, "Child 1 already exists\n");
        return;
    }
    parent->child_1 = create_node(scene);
}

void add_child_2(Node_G *parent, Scenario *scene) {
    if (parent->child_2 != NULL) {
        fprintf(stderr, "Child 2 already exists\n");
        return;
    }
    parent->child_2 = create_node(scene);
}


#endif





