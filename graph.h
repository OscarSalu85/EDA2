#include "queque.h"


typedef struct Node_G{
    Scenario scene;
    struct Node_G *father;
    struct Node_G *child_1;
    struct Node_G *child_2;    
}Node_G;

typedef struct Graph{
    Node_G *root;
}Graph;







