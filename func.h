#include <stdio.h>
#include "define.h"

const char two_byte_num[8][8] = {"０", "１", "２", "３", "４", "５", "６", "７"};
char pos_white_can_place[8][8];
char pos_black_can_place[8][8];

//盤面を初期化する
void set_board(char board[][8]){
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            if((row == 3 && col == 3) || (row == 4 && col == 4)) board[row][col] = WHITE;
            else if((row == 3 && col == 4) || (row == 4 && col == 3)) board[row][col] = BLACK;
            else board[row][col] = VACANT;
        }
    }
    // デバッグ
    // board[2][4] = WHITE;
    // board[3][4] = WHITE;
}

//指定座標にコマを置くことができるかを判定
char can_place(char board[][8], int row, int col, int player_c){ //（盤面，指定行番号，指定列番号，プレイヤー色）
    char place_v_info = 0;

    if(board[row][col] != VACANT) return -1; //指定座標にすでに他のコマが置かれていた場合
    else{
        for(int row_v = -1; row_v <= 1; row_v++){
            for(int col_v = -1; col_v <= 1; col_v++){
                if(row_v == 0 && col_v == 0) continue; //0,0方向は存在しないので除外
                for(int s = 1; s <= 7; s++){
                    int t_row = row + s*row_v;
                    int t_col = col + s*col_v;
                    char t_state = board[t_row][t_col];
                    if((t_row < 0 || t_row >= 8) || (t_col < 0 || t_col >= 8)) break;
                    if(s == 1){
                        if(t_state == player_c) break;
                    }
                    if(t_state == VACANT) break;
                    
                    if(t_state == player_c) place_v_info++;
                }
            }
        }
    }

    return place_v_info;
}

//コマを置くことのできる座標をすべて取得し pos_can_place に格納（置ける場合は引っくり返る方向情報もbitフラグで取得）
//また置くことのできる場所が一つでもあったかどうかを戻り値で返す（スキップ処理用）
int get_pos_can_place(char board[][8], char pos_can_place[][8], int player_c){
    char is_exist = 0;
    printf("get_pos_can_place called\n");
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            if(board[row][col] == VACANT){
                char can_place_info = can_place(board, row, col, player_c);
                pos_can_place[row][col] = can_place_info;
                if(can_place_info != 0) is_exist = 1;
            }
        }
    }
    return is_exist;
}


//盤面を表示
void print_board(char board[][8], char pos_can_place[][8], char player){
    printf("\n");
    printf("    ａ ｂ ｃ ｄ ｅ ｆ ｇ ｈ \n"); //列番号表示
    for(int row = 0; row < 8; row++){
        if(row != 0) printf("   -------------------------\n");
        printf("%s ", two_byte_num[row]); //行番号表示
        for(int col = 0; col < 8; col++){
            if(board[row][col] == WHITE){
                printf("|%s", WHITE_PIECE); //白表示
            }else if(board[row][col] == BLACK){
                printf("|%s", BLACK_PIECE); //黒表示
            }else{
                // if(can_place(board, row, col, player) != 0) printf("|可");
                if(pos_can_place[row][col] != 0) printf("|可");

                else printf("|  "); //空白表示
            }
        }
        printf("|\n");
    }

    printf("\n");
}


// char place_piece(char board[][8], int row, int col, int player_c){
//     if()
// }






// 参考
// https://qiita.com/t-yama-3/items/e3304cc4dce2193f9300