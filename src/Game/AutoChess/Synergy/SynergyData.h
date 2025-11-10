//---------------------------------------------------------------------------
//!	@file	SynergyData.h
//! @brief	シナジーデータの情報構造体
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <Game/AutoChess/Synergy/SynergyID.h>
//------------------------------------------------------
//! @brief シナジーの静的定義
//------------------------------------------------------
struct SynergyData
{
    SynergyID                id_;                  //!< 内部識別子
    std::string              name_;                //!< 表示名（UI用、例: "棋将共鳴"）
    std::vector<int>         level_thresholds_;    //!< 各段階の発動条件（駒数など、例: [2, 4, 6]）
    std::vector<std::string> descriptions_;        //!< 各段階の説明文（UI表示用）
    std::string              icon_path_;           //!< 表示用アイコンのパス（任意）

    //--------------------------------------------------
    //! @brief cereal によるシリアライズ定義
    //! @tparam Archive cerealのアーカイブ型
    //! @param ar cerealアーカイブへの参照
    //--------------------------------------------------
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(id_), CEREAL_NVP(name_), CEREAL_NVP(level_thresholds_), CEREAL_NVP(descriptions_), CEREAL_NVP(icon_path_));
    }
};
