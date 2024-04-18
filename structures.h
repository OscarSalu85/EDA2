#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

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
    Option options[MAX_OPTIONS];
    int n_options;
}Decision;

//Scenario
typedef struct Scenario{
    char name[MAX_CHAR];
    char description[MAX_CHAR];
    Decision decision[];
}Scenario;


//DATA
typedef struct Data{
    struct Character *character;
    struct Skills *skills[MAX_SKILLS];
    struct Enemy *enemies[MAX_ENEMIES];
    struct Option *last_option;
    struct Decision *last_decision;
    struct Scenario *current_scenario;
}Data;

#endif