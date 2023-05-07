#ifndef DEFINE_H
#define DEFINE_H


//マスの状態
enum {
    WHITE, //白が置かれている
    BLACK, //黒が置かれている
    VACANT //まだ置かれていない
};

//盤面表示の際の白コマ，黒コマ（表示崩れが起こる場合は変更してください）
#define WHITE_PIECE "○ "
#define BLACK_PIECE "● "
// #define WHITE_PIECE "白"
// #define BLACK_PIECE "黒"


//プレイヤーモード
enum {
    MANUAL = 1, //人力：コンソールから次の手を入力
    COM_RANDOM, //COMPUTER：選択できる場所からランダムに次の手を選ぶ
    COM_EDGE_HUNTER //COMPUTER：選択できる場所の中に端がある場合は絶対に端を選択，それ以外の場合はランダムに次の手を選ぶ
};


#define LOG 1 //盤面等のコンソール出力を行うか
#define ITERATION 1 //試合繰り返し回数

#define FILE_FLAG 0

#endif