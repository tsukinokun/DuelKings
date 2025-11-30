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
    std::string              type_name_;           //!< 駒の名前
    std::string              display_name_;        //!< UI表示名
    std::array<LevelData, 3> levels_;              //!< レベル123のステータス
    std::vector<SynergyID>   synergies_;           //!< 所属シナジー
    std::string              icon_path_;           //!< アイコンパス
    std::string              description_;         //!< 説明文
    float                    attack_interval_;     //!< 攻撃間隔
    float                    attack_range_;        //!< 射程距離
    int                      physical_defense_;    //!< 物理防御力
    float                    magical_defense_;     //!< 魔法防御力
    int                      price_;               //!< 購入価格

    //--------------------------------------------------
    //! @brief シリアライズ関数
    //! @param ar アーカイブ
    //! @note Cereal用のシリアライズ関数
    //--------------------------------------------------
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(type_name_),
           CEREAL_NVP(display_name_),
           CEREAL_NVP(levels_),
           CEREAL_NVP(synergies_),
           CEREAL_NVP(icon_path_),
           CEREAL_NVP(description_),
           CEREAL_NVP(attack_interval_),
           CEREAL_NVP(attack_range_),
           CEREAL_NVP(physical_defense_),
           CEREAL_NVP(magical_defense_),
           CEREAL_NVP(price_));
    }
};
