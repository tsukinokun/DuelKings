//---------------------------------------------------------------------------
//!	@file	SoundData.h
//! @brief	音データの構造体定義
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
struct SoundData
{
    std::unordered_map<std::string, std::string> se;     // 効果音ファイルのマップ
    std::unordered_map<std::string, std::string> bgm;    // BGMファイルのマップ

    //---------------------------------------------------------------------------
    //! @brief シリアライズ関数
    //! @param ar アーカイブオブジェクト
    //! @note cerealライブラリを使用してシリアライズを実装
    //---------------------------------------------------------------------------
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("se", se), cereal::make_nvp("bgm", bgm));
    }
};
