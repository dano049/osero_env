#ifndef DEFINE_H
#define DEFINE_H


//盤面の状態（白,黒，空）
enum {
    WHITE,
    BLACK,
    VACANT
};

//配置可能な場所か
enum {
    CAN_PLACE = 1,
    BAN
};

//盤面表示の際の白コマ，黒コマ
#define WHITE_PIECE "○ "
#define BLACK_PIECE "● "
// #define WHITE_PIECE "白"
// #define BLACK_PIECE "黒"


#endif