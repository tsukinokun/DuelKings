//---------------------------------------------------------------------------
//!	@file	PiecePool.cpp
//! @brief	駒プール
//! @author 山﨑愛
//---------------------------------------------------------------------------
#include "PiecePool.h"
#include <Game/AutoChess/system/GameConst.h>
#include <Game/AutoChess/Piece/Piece.h>
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
        {          "ChessKing", piece_init_stock[0]}  //キング}
    };
}
//---------------------------------------------------------------------------
//!	プールから抽選を行って、駒を1体取得する
//---------------------------------------------------------------------------
std::shared_ptr<Piece> PiecePool::GetRandomPiece()
{
    std::vector<std::string> available;

    // 在庫が1以上あるユニット名を収集
    for(const auto& [name, count] : piece_pool_[0]) {
        if(count > 0)
            available.push_back(name);
    }

    // 抽選可能なユニットが存在しない場合は nullptr を返す
    if(available.empty())
        return nullptr;

    // 抽選対象からランダムに1体選択
    std::uniform_int_distribution<size_t> dist(0, available.size() - 1);
    std::mt19937                          rng_{std::random_device{}()};    //!< ランダム抽選用の乱数生成器
    std::string                           selected = available[dist(rng_)];

    // 選ばれたユニットの在庫数を1減らす
    piece_pool_[0][selected]--;

    // ユニットインスタンスを生成して返す（RAIIによる所有権管理）
    return PieceFactory::CreatePiece(selected);
}
