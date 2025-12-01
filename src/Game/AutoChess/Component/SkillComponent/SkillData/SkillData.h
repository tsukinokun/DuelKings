//---------------------------------------------------------------------------
//!	@file	SkillData.h
//! @brief	スキルデータの情報構造体
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
//------------------------------------------------------
//! @brief スキルの静的定義
//------------------------------------------------------
struct SkillData
{
    float       max_cool_doen_time_   = 0.0f;                          //!< クールダウン最大値
    std::string skill_component_name_ = "";                            //!< つけるスキルコンポーネントの名前
    std::string skill_name_           = "ActiveSkill";                 //!< スキル名
    std::string skill_description_    = "Active Skill Description";    //!< スキル説明文

    //--------------------------------------------------
    //! @brief シリアライズ関数
    //! @param ar アーカイブ
    //! @note Cereal用のシリアライズ関数
    //--------------------------------------------------
    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(CEREAL_NVP(max_cool_doen_time_), CEREAL_NVP(skill_component_name_), CEREAL_NVP(skill_name_), CEREAL_NVP(skill_description_));
    }
};
