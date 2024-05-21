#ifndef STRUCTURES_H
#define STRUCTURES_H
#define MAX_CHAR_DESC 1024
#define MAX_CHAR_NAME 20
#define MAX_ENEMIES 3
#define MAX_OPTIONS 3
#define MAX_DECISIONS 3
#define MAX_SKILLS 4
#define MOD_NUMBER 3
#define SKILL_DATA_SIZE 10
//Skills
typedef struct Skills{
    char name[MAX_CHAR_NAME];
    char description[MAX_CHAR_DESC];
    int duration;
    int damage;
    int modifiers[MOD_NUMBER];
}Skills;

//Character
typedef struct Character{
    char name[MAX_CHAR_NAME];
    int hp;
    int atk;
    int def;
    Skills *skill[MAX_SKILLS];
}Character;

//Enemy
typedef struct Enemy{
    char name[MAX_CHAR_NAME];
    int hp;
    int atk;
    int def;
    Skills skill[4];
}Enemy;

//Option
typedef struct Option{
    char r_text[MAX_CHAR_DESC];
    char n_text[MAX_CHAR_DESC];
    Enemy *enemies[MAX_ENEMIES];
}Option;

//Decision
typedef struct Decision{
    char question[MAX_CHAR_DESC];
    int n_options;
    Option *options[MAX_OPTIONS];
}Decisions;

//Scenario
typedef struct Scenario{
    char name[MAX_CHAR_NAME];
    char description[MAX_CHAR_DESC];
    char image[MAX_CHAR_DESC];
    Decisions *decision;
    
}Scenario;


//DATA
typedef struct Data{
    Character *character;
    Scenario *current_scenario;
}Data;

#endif