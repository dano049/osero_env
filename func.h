#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>
#include "define.h"

char chg_vector_to_id(int row_v, int col_v);

//オセロ盤面の行番号表示用
const char two_byte_num[8][8] = {"１", "２", "３", "４", "５", "６", "７", "８"};

//ビットフラグ用
//それぞれ 0000 0001 / 0000 0010 / 0000 0100 / 0000 1000 / 0001 0000 / 0010 0000 / 0100 0000 / 1000 0000
const int bit_ar[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

//ID -> 配列 の変換用
const int id_to_vector[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}};


char selectable_move[64][2];
int selectable_move_num;


//盤面を初期化する
void set_board(char board[][8]){
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            if((row == 3 && col == 3) || (row == 4 && col == 4)) board[row][col] = WHITE;
            else if((row == 3 && col == 4) || (row == 4 && col == 3)) board[row][col] = BLACK;
            else board[row][col] = VACANT;
        }
    }
}


//指定座標にコマを置くことができるかを判定，置いた場合に引っくり返る方向をビットフラグとして返す（置けない場合は0を返す）
unsigned char can_put(char board[][8], int row, int col, int player_c){ //（盤面，指定行番号，指定列番号，プレイヤー色）
    unsigned char put_v_info = 0;

    if(board[row][col] != VACANT) return 0; //指定座標にすでに他のコマが置かれていた場合
    else{
        for(int row_v = -1; row_v <= 1; row_v++){ //走査の上下方向（-1：上，0：指定なし，1：下）
            for(int col_v = -1; col_v <= 1; col_v++){ //走査の左右方向（-1：左，0：指定なし，1：右）
                if(row_v == 0 && col_v == 0) continue; //0,0方向は存在しないので除外
                for(int s = 1; s <= 8; s++){
                    int t_row = row + s*row_v; //走査座標の行番号
                    int t_col = col + s*col_v; //走査座標の列番号
                    if((t_row < 0 || t_row >= 8) || (t_col < 0 || t_col >= 8)) break; //走査番地が盤面の外までいった場合次の方向へ

                    char target_state = board[t_row][t_col]; //走査座標の状態を取得 (WHITE / BLACK / VACANT)

                    if(target_state == VACANT) break; //途中に空白マスがある場合ひっくり返せない．次の方向へ
                    
                    if(target_state == player_c){
                        if(s == 1) break; //現在の方向だと，置こうとしている座標に隣接して自身のコマが置かれているため，ひっくり返せない．次の方向へ
                        else{ //当該方向へ引っくり返せる場合
                            put_v_info |= bit_ar[chg_vector_to_id(row_v, col_v)]; //当該方向のフラグを立てる
                        }
                    }
                }
            }
        }
    }

    return put_v_info;
}

//コマを置けるかを座標をすべてのマスで確認し pos_can_put に格納（置ける場合は引っくり返る方向情報もbitフラグで取得）
//また置くことのできる場所が一つでもあったかどうかを戻り値で返す（スキップ処理用）
char get_pos_can_put(char board[][8], char pos_can_put[][8], int player_c){
    char is_exist = 0;
    selectable_move_num = 0;
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            unsigned char can_put_info = can_put(board, row, col, player_c);
            pos_can_put[row][col] = can_put_info;
            if(can_put_info != 0){
                is_exist = 1;
                selectable_move[selectable_move_num][0] = row;
                selectable_move[selectable_move_num][1] = col;
                selectable_move_num++;
            }
        }
    }

    return is_exist;
}


//方向情報を0~7の方向IDへ変換
char chg_vector_to_id(int row_v, int col_v){
    if(row_v == -1){
        if(col_v == -1) return 0; //(row_v, col_v) = (-1, -1)
        else if(col_v == 0) return 1; //(-1, 0)
        else return 2; //(-1, 1)
    }else if(row_v == 0){
        if(col_v == 1) return 3; //(0, 1)
        else return 7; //(0, -1)
    }else if(row_v == 1){
        if(col_v == 1) return 4; //(1, 1)
        else if(col_v == 0) return 5; //(1, 0)
        else return 6; //(1, -1)
    }
}


//盤面を表示
void print_board(char board[][8], char pos_can_put[][8]){
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
                // if(can_put(board, row, col, player) != 0) printf("|可");
                if(pos_can_put != NULL){
                    if(pos_can_put[row][col] != 0) printf("|可"); //現在の番手のプレイヤーがおける場所を表示
                    else printf("|  "); //空白表示
                }else printf("|  "); //空白表示
            }
        }
        printf("|\n");
    }

    printf("\n");
}


void input_next_move(int next_move[2]){ //(row, col) 入力順と逆なので注意
    printf("please input your next move\n");
    printf("usage ex) c5\n");

    //列番号(column)の入力
    while(1){
        char input_str[100];
        printf("input: ");
        scanf("%s", input_str);
        int input_id = input_str[0] - 97; //asciiコードを基準(a:97 ~ h:104) -> (a:0 ~ h:7)
        
        if(input_id >= 0 && input_id <= 7){ //正常値の場合
            next_move[1] = input_id;
        }else{
            printf("\n!! input error !!\n");
            printf("please input column id again (a ~ h)\n");
            continue;
        }

        int row_id = (int)input_str[1];
        row_id = row_id - 1 - 48; //(0~7)
        printf("row_id:%d\n", row_id);
        
        if(row_id >= 0 && row_id <= 7){ //正常値の場合
            next_move[0] = row_id;
            break;
        }
        else{
            printf("\n!! input error !!\n");
            printf("please input row id again (1 ~ 8)\n");
        }
    }

    printf("-----------------\n\n");
}



//指定座標にコマを置き，挟んだコマを引っくり返す
void put_and_reverse(char board[][8], unsigned char put_v_info, int row, int col, char player_c){
    board[row][col] = player_c; //まず指定座標にコマを置く

    //引っくり返す処理
    for(int i = 0; i <= 7; i++){ //8方向 繰り返し
        if((put_v_info & bit_ar[i]) == bit_ar[i]){ //当該方向のフラグが立っていた場合
            int row_v = id_to_vector[i][0]; //方向を取得
            int col_v = id_to_vector[i][1]; //方向を取得
            for(int s = 1; s <= 8; s++){
                int t_row = row + s*row_v; //走査座標の行番号
                int t_col = col + s*col_v; //走査座標の列番号
                char target_state = board[t_row][t_col]; //走査座標の状態を取得 (WHITE / BLACK / VACANT)
                if((t_row < 0 || t_row >= 8) || (t_col < 0 || t_col >= 8)) abort(); //エラー（デバッグ）
                if(target_state == VACANT) abort(); //エラー（デバッグ）
                
                if(target_state == player_c){ //挟んだもう一端に到達した場合
                    break; //次の方向へ
                }else{
                    board[t_row][t_col] = player_c; //コマを引っくり返す
                }
            }
        }
    }
}

//勝敗を判定
int judge_winner(char board[][8]){
    int white_cnt = 0;
    int black_cnt = 0;

    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            char state = board[row][col];
            if(state == WHITE) white_cnt++;
            else if(state == BLACK) black_cnt++;
        }
    }

    printf(":========== RESULT ========== \n");
    printf("WHITE:%d\n", white_cnt);
    printf("BLACK:%d\n", black_cnt);

    char winner;

    if(white_cnt > black_cnt){
        printf("WHITE WIN\n");
        return WHITE;
    }else if(black_cnt > white_cnt){
        printf("BLACK WIN\n");
        return BLACK;
    }else if(white_cnt == black_cnt){
        printf("DRAW\n");
        return BLACK + 1;
    }
}


//COM動作：ランダム選択
void com_random(int next_move[2]){
    char move_id = rand() % selectable_move_num; //置くことのできる場所からランダムに手を選択
    next_move[0] = selectable_move[move_id][0];
    next_move[1] = selectable_move[move_id][1];
}



//COM動作：隅と端を選択できる場合は絶対に選択する，それ以外の場合はランダムに選択する
void com_edge_hunter(int next_move[2]){

    //隅が選択できないか最優先で確認する（複数ある場合は最初に見つかったものを選択）
    for(int i = 0; i < selectable_move_num; i++){
        int row = selectable_move[i][0];
        int col = selectable_move[i][1];
        if((row == 0 && col == 0) || (row == 0 && col == 7) || (row == 7 && col == 0) || (row == 7 && col == 7)){
            next_move[0] = row;
            next_move[1] = col;
            return;
        }
    }

    //次に端が選択できないか確認する（複数ある場合は最初に見つかったものを選択）
    for(int i = 0; i < selectable_move_num; i++){
        int row = selectable_move[i][0];
        int col = selectable_move[i][1];
        if((row == 0) || (row == 7) || (col == 0) || (col == 7)){
            next_move[0] = row;
            next_move[1] = col;
            return;
        }
    }

    //隅，端が見つからなかった場合，ランダムに選択
    com_random(next_move);
}




#endif

// 参考
// 二次元配列引数：https://qiita.com/t-yama-3/items/e3304cc4dce2193f9300
// bitフラグ：https://programming.pc-note.net/c/bit2.html
// scanfの数字入力へ文字が入力された場合の無限ループ対策：https://daeudaeu.com/c-scanf-infinity/