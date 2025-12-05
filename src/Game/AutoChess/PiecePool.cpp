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
std::array<std::unordered_map<std::string, int>, RARITIES> PiecePool::piece_pool_ = {};
//---------------------------------------------------------------------------
//!	コンストラクタ
//---------------------------------------------------------------------------
void PiecePool::Init(const PieceRepository& piece_repo)
{
    //各駒を初期化
    auto piece_datas = piece_repo.GetAllPieces();
    for(int i = 0; i < piece_datas.size(); i++) {
        std::string type_name             = piece_datas[i].type_name_;        //駒の種類名を取得
        int         price                 = piece_datas[i].price_;            //駒の価格を取得
        piece_pool_[price - 1][type_name] = {piece_init_stock[price - 1]};    //価格に応じたプールを初期化
    }

    //piece_pool_[0] = {
    //    {"JapaneseChessKing",   piece_init_stock[0]}, //王将
    //    {"JapaneseChessKnight", piece_init_stock[0]}, //桂馬
    //    {"JapaneseChessPawn",   piece_init_stock[0]}, //歩兵
    //    {"JapaneseChessLance",  piece_init_stock[0]}, //香車
    //    {"JapaneseChessSilver", piece_init_stock[0]}, //銀将
    //    {"JapaneseChessGold",   piece_init_stock[0]}, //金将
    //    {"JapaneseChessBishop", piece_init_stock[0]}, //角行
    //    {"JapaneseChessRook",   piece_init_stock[0]}, //飛車
    //    {"ChessPawn",           piece_init_stock[0]}, //ポーン
    //    {"ChessKnight",         piece_init_stock[0]}, //ナイト
    //    {"ChessBishop",         piece_init_stock[0]}, //ビショップ
    //    {"ChessRook",           piece_init_stock[0]}, //ルーク
    //    {"ChessQueen",          piece_init_stock[0]}, //クイーン
    //    {"ChessKing",           piece_init_stock[0]}, //キング
    //    {"MakrukBia",           piece_init_stock[0]}, //ビア
    //    {"MakrukKhon",          piece_init_stock[0]}, //コーン
    //    {"MakrukKhun",          piece_init_stock[0]}, //クン
    //    {"MakrukMa",            piece_init_stock[0]}, //マーク
    //    {"MakrukMet",           piece_init_stock[0]}, //メット
    //    {"MakrukRuea",          piece_init_stock[0]}, //ルア
    //    {"ChaturangaAsva",      piece_init_stock[0]}, //アスヴァ
    //    {"ChaturangaRatha",     piece_init_stock[0]}, //ラタ
    //    {"ChaturangaGaja",      piece_init_stock[0]}, //ガジャ
    //    {"ChaturangaPadati",    piece_init_stock[0]}, //パダティ
    //    {"ChaturangaMantri",    piece_init_stock[0]}, //マントリ
    //    {"ChaturangaRaja",      piece_init_stock[0]}, //ラジャ
    //};
}

//---------------------------------------------------------------------------
//! レアリティ抽選関数
//---------------------------------------------------------------------------
int PiecePool::DrawRarity(int agent_level)
{
    // 乱数生成器を静的に保持（毎回初期化せずに使い回す）
    static std::mt19937 rng{std::random_device{}()};

    // 1〜100 の範囲で一様乱数を生成する分布を定義
    std::uniform_int_distribution<int> dist(1, 100);

    // 実際に乱数を生成（抽選の「サイコロ振り」）
    int roll       = dist(rng);    // 1〜100の乱数
    int cumulative = 0;            // 累積確率を保持する変数

    // 現在のレベルに対応する確率行を取得
    // probabilityTable は [LEVELS][RARITIES] の二次元配列
    const std::array<int, RARITIES>& probability = piece_offer_rates.at(agent_level - 1);

    // 各レアリティの確率を累積して、乱数がどこに収まるか判定
    for(int rarity = 0; rarity < RARITIES; ++rarity) {
        cumulative += probability[rarity];    // 累積確率を加算
        if(roll <= cumulative) {
            // 乱数がこの範囲に収まったら、そのレアリティを返す
            return rarity + 1;    // ★1〜★5 に対応
        }
    }

    // 万が一確率が合わなかった場合の保険（通常は到達しない）
    return 1;    // fallback として ★1 を返す
}

//---------------------------------------------------------------------------
//!	プールから抽選を行って、駒を1体取得する
//---------------------------------------------------------------------------
PieceInfo PiecePool::GetRandomPiece(int agent_level)
{
    std::vector<std::string> available;

    int index = PiecePool::DrawRarity(agent_level);

    // 在庫が1以上あるユニット名を収集
    for(const auto& [name, count] : piece_pool_[index - 1]) {
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
    piece_pool_[index - 1][selected]--;

    PieceInfo piece;
    piece.SetTypeName(selected);

    // ピース情報のポインタを返す（RAIIによる所有権管理）
    return piece;
}
