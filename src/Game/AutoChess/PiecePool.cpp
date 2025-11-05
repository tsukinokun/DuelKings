//---------------------------------------------------------------------------
//!	@file	PiecePool.cpp
//! @brief	駒プール
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PiecePool.h"
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Info/PieceInfo.h>
#include <Game/AutoChess/PieceFactory.h>
//staticメンバ変数の定義
std::array<std::unordered_map<std::string, int>, 5> PiecePool::piece_pool_ = {};
//---------------------------------------------------------------------------
//!	コンストラクタ
//---------------------------------------------------------------------------
void PiecePool::Init()
{
    //各レアリティごとに駒の名前と在庫数を初期化
    piece_pool_[0] = {
        {  "JapaneseChessKing", piece_init_stock[0]}, //王将
        {"JapaneseChessKnight", piece_init_stock[0]}, //桂馬
        {  "JapaneseChessPawn", piece_init_stock[0]}, //歩兵
        { "JapaneseChessLance", piece_init_stock[0]}, //香車
        {"JapaneseChessSilver", piece_init_stock[0]}, //銀将
        {  "JapaneseChessGold", piece_init_stock[0]}, //金将
        {"JapaneseChessBishop", piece_init_stock[0]}, //角行
        {  "JapaneseChessRook", piece_init_stock[0]}, //飛車
        {          "ChessPawn", piece_init_stock[0]}, //ポーン
        {        "ChessKnight", piece_init_stock[0]}, //ナイト
        {        "ChessBishop", piece_init_stock[0]}, //ビショップ
        {          "ChessRook", piece_init_stock[0]}, //ルーク
        {         "ChessQueen", piece_init_stock[0]}, //クイーン
        {          "ChessKing", piece_init_stock[0]}, //キング
        {          "MakrukBia", piece_init_stock[0]}, //ビア
        {         "MakrukKhon", piece_init_stock[0]}, //コーン
        {         "MakrukKhun", piece_init_stock[0]}, //クン
        {           "MakrukMa", piece_init_stock[0]}, //マーク
        {          "MakrukMet", piece_init_stock[0]}, //メット
        {         "MakrukRuea", piece_init_stock[0]}, //ルア
        {     "ChaturangaAsva", piece_init_stock[0]}, //アスヴァ
        {    "ChaturangaRatha", piece_init_stock[0]}, //ラタ
        {     "ChaturangaGaja", piece_init_stock[0]}, //ガジャ
        {   "ChaturangaPadati", piece_init_stock[0]}, //パダティ
        {   "ChaturangaMantri", piece_init_stock[0]}, //マントリ
        {     "ChaturangaRaja", piece_init_stock[0]}, //ラジャ
    };
}
//---------------------------------------------------------------------------
//!	プールから抽選を行って、駒を1体取得する
//---------------------------------------------------------------------------
PieceInfo PiecePool::GetRandomPiece()
{
    std::vector<std::string> available;

    // 在庫が1以上あるユニット名を収集
    for(const auto& [name, count] : piece_pool_[0]) {
        if(count > 0)
            available.push_back(name);
    }

    // 抽選可能なユニットが存在しない場合は空の情報を返す
    if(available.empty())
        return PieceInfo();

    // 抽選対象からランダムに1体選択
    std::uniform_int_distribution<size_t> dist(0, available.size() - 1);
    std::mt19937                          rng_{std::random_device{}()};    //!< ランダム抽選用の乱数生成器
    std::string                           selected = available[dist(rng_)];

    // 選ばれたユニットの在庫数を1減らす
    piece_pool_[0][selected]--;

    PieceInfo piece;
    piece.SetTypeName(selected);

    // ピース情報のポインタを返す（RAIIによる所有権管理）
    return piece;
}
