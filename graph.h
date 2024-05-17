#include "queque.h";

typedef struct Node{
    Scenario scene;
    struct Node *father;
    struct Node *child_1;
    struct Node *child_2;    
}Node;

typedef struct Graph{
    Node *root;
}Graph;


