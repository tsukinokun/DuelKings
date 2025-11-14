//---------------------------------------------------------------------------
//!	@file	GameConst.h
//! @brief	定数群ファイル
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
const float SQUARE_SIZE = 1.0f;    //マスのxz軸の大きさ
const float SQUARE_HALF = 0.5f;    //マスのxz軸の半分の大きさ
//ゲームプレイ人数
constexpr int AGENT_NUM = 8;    //プレイ人数
//フェーズの長さ
constexpr float SETUP_PHASE_DURATION  = 15.0f;    // 設置フェーズの長さ（秒）
constexpr float BATTLE_PHASE_DURATION = 10.0f;    // 戦闘フェーズの長さ（秒）
//ピースの最大レベル
constexpr int MAX_PIECE_LEVEL = 3;
//レベルアップに必要なピース数
constexpr int LEVEL_UP_REQUIRED_PIECE_COUNT = 3;
//経験値テーブル
const std::vector<int>   exp_table        = {0, 1, 2, 4, 8, 18, 28, 38, 48};
const std::array<int, 5> piece_init_stock = {25, 20, 15, 10, 10};    //各レアリティごとの初期在庫数
//Agentの最大HP
int constexpr MAX_AGENT_HP = 100;
//---------------------------------------------------------------------------
//  駒提供割合の定数
//  [レベル][コスト] = 出現率（%）
//  コストは 1G〜5G に対応（インデックス 0〜4）
//--------------------------------　-------------------------------------------
constexpr std::array<std::array<int, 5>, 10> piece_offer_rates = {
    {
     {100, 0, 0, 0, 0},      // Lv1
        {70, 30, 0, 0, 0},      // Lv2
        {60, 35, 5, 0, 0},      // Lv3
        {50, 35, 15, 0, 0},     // Lv4
        {45, 35, 23, 2, 0},     // Lv5
        {33, 30, 30, 7, 0},     // Lv6
        {30, 30, 30, 10, 0},    // Lv7
        {24, 30, 30, 15, 1},    // Lv8
        {22, 30, 25, 20, 3},    // Lv9
        {19, 25, 25, 25, 6},    // Lv10
    }
};
