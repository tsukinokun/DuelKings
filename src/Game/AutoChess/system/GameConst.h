//---------------------------------------------------------------------------
//!	@file	GameConst.h
//! @brief	定数群ファイル
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
const float SQUARE_SIZE = 1.0f;    //マスのxz軸の大きさ
const float SQUARE_HALF = 0.5f;
//経験値テーブル
const std::vector<int>   exp_table        = {0, 1, 2, 4, 8, 18, 28, 38, 48};
const std::array<int, 5> piece_init_stock = {25, 20, 15, 10, 10};    //各レアリティごとの初期在庫数
