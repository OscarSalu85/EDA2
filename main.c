#include <stdio.h>
#include "structures.h"
#include "game.h"
#include "cJSON.c"

int main(){
    Data *data;
    create_data(&data);
    while(1){
        //Menu inicial
        int main_menu_choice = main_menu(data);
        if(main_menu_choice == 1){
            //continue_game(data);
        }
        else if(main_menu_choice == 2){
            new_game(data);
        }
        else if(main_menu_choice == 3){
            configure_menu(data);
        }
    }
}


//gcc -o main main.c cJSON.c