//---------------------------------------------------------------------------
//!	@file	JapaneseChessPawnSkill.h
//! @brief	歩兵のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(JapaneseChessPawnSkill);

class JapaneseChessPawnSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(JapaneseChessPawnSkill, u8"歩兵のスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    JapaneseChessPawnSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3>   PHYSICAL_DEFENSE_BONUS_ = {10, 15, 20};       //物理防御のレベルごとのバフ
    const std::array<float, 3> DURATION_OF_EFFECT_     = {2.5, 3.5, 4.5};    //効果時間
    float                      effect_timer_           = 0.0f;               //効果時間タイマー
};

CEREAL_REGISTER_TYPE(JapaneseChessPawnSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JapaneseChessPawnSkill)
