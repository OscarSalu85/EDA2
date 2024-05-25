
#include "structures.h"
#include <stdlib.h>
#ifndef QUEQUE_H
#define QUEQUE_H
#define NUM_TURNS 50

typedef struct Turn{
    char *name;
    int type;
    Enemy *enemy;
    struct Turn *next;
}Turn;

typedef struct Queue{
    Turn *first;
    Turn *last;
}Queue;

void AddElement(Turn *turn, Queue *queue){
    if(queue->first == NULL){
        queue->first = turn;
        queue->last = turn;
    }
    else if(queue->first == queue->last){
        queue->last = turn;
        queue->first->next = turn;
    }
    else{
        queue->last->next = turn;
        queue->last = turn;
    }
}

Queue* createQueue(Character *character, Enemy *enemies[MAX_ENEMIES],Queue *queue,int num_enemies){
    queue->first = NULL;
    queue->last = NULL;
    printf("Order combat:\n");
    for(int i = 0; i < NUM_TURNS; i++){
        Turn *turn = malloc(sizeof(Turn));
        turn->enemy = NULL;
        turn->name = NULL;
        turn->next = NULL;
        turn->type = -1;

        int random = rand()%2;
        if(random == 0){
            turn->enemy = NULL;
            turn->name = "character";
            turn->type = 0;
        }
        else{
            if(num_enemies != 0){
                random = rand()%(num_enemies);
                turn->enemy = enemies[random];
                turn->type = 1;
                turn->name = enemies[random]->name;
            }
        }
        AddElement(turn,queue);
        //printf("%s",queue->last->name);
    }
    return queue;
}
#endif