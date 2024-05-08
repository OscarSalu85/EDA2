#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_CHAR 100
#define MAX_ENEMIES 3
#define MAX_OPTIONS 3
#define MAX_DECISIONS 3
#define MAX_SKILLS 4
//Skills
typedef struct Skills{
    char name[MAX_CHAR];
    char description[MAX_CHAR];
    int duration;
    char modifiers;
}Skills;

//Character
typedef struct Character{
    char name[MAX_CHAR];
    int hp;
    int atk;
    int def;
    Skills skill[4];
}Character;

//Enemy
typedef struct Enemy{
    char name[MAX_CHAR];
    int hp;
    int atk;
    int def;
}Enemy;

//Option
typedef struct Option{
    char r_text[MAX_CHAR];
    char n_text[MAX_CHAR];
    Enemy enemies[MAX_ENEMIES];
}Option;

//Decision
typedef struct Decision{
    char question[MAX_CHAR];
    int n_options;
    Option options[n_options];
    
}Decisions;

//Scenario
typedef struct Scenario{
    char name[MAX_CHAR];
    char description[MAX_CHAR];
    char image[MAX_CHAR];
    int n_decision;
    Decisions decision[n_decision];
    
}Scenario;


//DATA
typedef struct Data{
    Character *character;
    Skills *skills[MAX_SKILLS];
    Option *last_option;
    Decisions *last_decision;
    Scenario *current_scenario;
}Data;

#endif