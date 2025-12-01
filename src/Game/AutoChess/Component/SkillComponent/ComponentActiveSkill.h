//---------------------------------------------------------------------------
//!	@file	ComponentActiveSkill.h
//! @brief	アクティブスキルのベースコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>

USING_PTR(ComponentActiveSkill);

class ComponentActiveSkill : public Component
{
public:
    BP_COMPONENT_DECL(ComponentActiveSkill, u8"アクティブスキルのベースコンポーネントクラス");

    virtual void Init() override;    //!< 初期化

    //---------------------------------------------------------
    //  スキル発動可能かを返す関数
    //!  @return スキル発動可能ならtrue、不可ならfalse
    //---------------------------------------------------------
    bool CanActivate() const;

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    virtual void Activate();

    //---------------------------------------------------------
    // MPを増加させる関数
    //! @param amount [in] 増加させるMP量
    //---------------------------------------------------------
    void AddMP(int amount);

    //---------------------------------------------------------
    // MPを取得する関数
    //! @retval 現在のスキルMP
    //---------------------------------------------------------
    int GetMP() const;

protected:
    int         mp_                 = 0;                             //!< スキルMP
    float       max_cool_doen_time_ = 15.0f;                         //!< クールダウン最大値
    float       cool_doen_time_     = 0.0f;                          //!< クールダウン時間
    std::string skill_name_         = "ActiveSkill";                 //!< スキル名
    std::string skill_description_  = "Active Skill Description";    //!< スキル説明文
};

CEREAL_REGISTER_TYPE(ComponentActiveSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ComponentActiveSkill)
