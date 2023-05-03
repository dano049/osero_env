#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "func.h"

//===============
//オセロゲーム
//===============

const char PLAYER_COLOR[2][8] = {"WHITE", "BLACK"}; 

int main(){

    printf("==== OSERO START ====\n");

    char board[8][8]; //オセロ盤面
    set_board(board);

    //先手を決定，表示
    srand((unsigned int)time(NULL));
    char current_player = rand() % 2;
    printf("First_Player : %s\n", PLAYER_COLOR[current_player]);
    printf("=====================\n\n");

    while(1){
        printf("Player : %s\n", PLAYER_COLOR[current_player]);
        break;
    }

    char pos_can_place[8][8];
    get_pos_can_place(board, pos_can_place, current_player);
    print_board(board, pos_can_place, current_player);


    // while(1){

    // }
    









    return 0;
}