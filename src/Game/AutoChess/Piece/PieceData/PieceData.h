//---------------------------------------------------------------------------
//!	@file	PieceData.h
//! @brief	ピースデータの情報構造体
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <Game/AutoChess/Synergy/SynergyID.h>
#include <Game/AutoChess/Piece/PieceData/LevelData.h>
//------------------------------------------------------
//! @brief 駒の静的定義（レベル別ステータス含む）
//------------------------------------------------------
struct PieceData
{
    std::string              type_name;       //!< 駒の名前
    std::string              display_name;    //!< UI表示名
    std::array<LevelData, 3> levels;          //!< レベル1〜3のステータス
    std::vector<SynergyID>   synergies;       //!< 所属シナジー
    std::string              icon_path;       //!< アイコンパス
    std::string              description;     //!< 説明文
};
