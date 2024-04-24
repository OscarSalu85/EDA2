#include <stdio.h>
#include "structures.h"
#include "game.c"
int main(){
    Data* data;
    while(1){
        //Menu inicial
        int main_menu_choice = main_menu(data);
        if(main_menu_choice == 1){
            continue_game(data);
        }
        else if(main_menu_choice == 2){
            new_game(data);
        }
        else if(main_menu_choice == 3){
            configure(data);
        }
        
    }
}


