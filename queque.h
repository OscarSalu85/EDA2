#ifndef QUEQUE_H
#define QUEQUE_H
#include "structures.h"
#include <stdlib.h>

#define NUM_TURNS 20

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

void AddElement(Turn turn, Queue queue){
    if(queue.first == NULL){
        queue.first = &turn;
        queue.last = &turn;
    }
    else if(queue.first == queue.last){
        queue.last = &turn;
        turn.next = queue.first;
    }
    else{
        turn.next = queue.last;
        queue.last = &turn;
    }
}

Queue createQueue(Character *character, Enemy *enemies){
    Queue queue;
    for(int i = 0; i < NUM_TURNS; i++){
        Turn turn;

        int random = rand()%2;
        if(random == 0){
            turn.enemy = NULL;
            turn.name = "character";
            turn.type = 0;
        }
        else{
            random = rand()%(sizeof(enemies)/sizeof(Enemy));
            turn.enemy = &enemies[random];
            turn.type = 1;
            turn.name = enemies[random].name;
        }
        turn.next = NULL;
        AddElement(turn,queue);
    }
    return queue;
}

#endif