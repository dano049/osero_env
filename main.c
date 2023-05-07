#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "func.h"

//=====================================
//オセロゲーム 機械学習の環境用に作成
//=====================================

const char PLAYER_COLOR[2][8] = {"WHITE", "BLACK"}; 
const char OPERATION_MODE[2] = {COM_RANDOM, COM_EDGE_HUNTER}; //(WHITE, BLACK) プレイヤーの動作モードを設定

int main(){

    printf("==== OSERO START ====\n");

    char board[8][8]; //オセロ盤面(8x8)
    set_board(board); //盤面を初期化

    //先手を決定，表示
    srand((unsigned int)time(NULL));
    char current_player = rand() % 2; //先手をランダムに決定
    printf("First_Player : %s\n", PLAYER_COLOR[current_player]);
    printf("=====================\n\n");

    print_board(board, NULL); //現在の盤面を表示

    int vacant_place_num = 64; //残り空きマス数
    int turn = 1; //現在のターン数
    char skip_flag = 0;

    while(vacant_place_num > 0){
        
        printf("Turn : %d\n", turn); //現在のターン数表示
        printf("Player : %s", PLAYER_COLOR[current_player]); //現在番手のプレイヤー表示
        if(current_player == WHITE) printf("  %s\n", WHITE_PIECE);
        else printf("  %s\n", BLACK_PIECE);
        print_board(board, NULL); //現在の盤面を表示
        
        char pos_can_put[8][8]; //全座標の配置可否情報（置くことができるか，置ける場合引っくり返せる方向もビットフラグで保持）
        char can_put; //コマを置ける場所があるかどうかのフラグ
        can_put = get_pos_can_put(board, pos_can_put, current_player); //置ける場所があるかどうか判定，また全座標の情報をpos_can_putに格納

        if(can_put == 1){ //置く場所がある場合
            int next_move[2]; //次に置こうとしている座標(row, col)
            unsigned char put_v_info; //置こうとしている座標の情報（置けるかどうか，引っくり返る向き）
            while(1){
                if(OPERATION_MODE[current_player] == MANUAL){ //手動入力モードの場合
                    input_next_move(next_move); //次の手を入力
                    put_v_info = pos_can_put[next_move[0]][next_move[1]];
                    if(put_v_info != 0) break; //置ける場合は入力終了
                    else{ //配置場所が不正な場合
                        printf("!! position error !!\n");
                        printf("can not put this position\n");
                        printf("please input your next move again\n\n");
                    }
                }else if(OPERATION_MODE[current_player] == COM_RANDOM){ //COMモード(RANDOM)の場合
                    printf("COM(RANDOM) TURN\n");
                    com_random(next_move);
                    put_v_info = pos_can_put[next_move[0]][next_move[1]];
                    if(put_v_info != 0) break;
                }else if(OPERATION_MODE[current_player] == COM_EDGE_HUNTER){ //COMモード(隅・端優先)の場合
                    printf("COM(EDGE_HUNTER) TURN\n");
                    com_edge_hunter(next_move);
                    put_v_info = pos_can_put[next_move[0]][next_move[1]];
                    if(put_v_info != 0) break;
                }
                
            }

            // printf("v_info:%d\n", put_v_info); //デバッグ
            put_and_reverse(board, put_v_info, next_move[0], next_move[1], current_player); //指定座標のマスにコマを置き，挟んだコマを引っくり返す

            vacant_place_num--; //残り空きマス数を減らす
            skip_flag = 0;

        }else{ //どこにも置けない場合 -> スキップ
            printf("!! SKIP !!\n");
            if(skip_flag == 1){ //両者連続でスキップとなった場合
                break; //試合終了
            }
            skip_flag = 1; 
        }

        current_player = !current_player; //番手の交代

        turn++;
        printf("\n===========================================================\n");
    }

    printf("\n=================== GAME SET =================================================\n\n");

    judge_winner(board);


    return 0;
}