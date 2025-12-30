//---------------------------------------------------------------------------
//!	@file	ChaturangaMantriSkill.h
//! @brief	マントリのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChaturangaMantriSkill);
class Piece;    // 前方宣言
class ChaturangaMantriSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChaturangaMantriSkill, u8"マントリのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChaturangaMantriSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    //---------------------------------------------------------
    // 攻撃時の効果処理
    //---------------------------------------------------------
    void ApplyEffectToTarget();

private:
    const std::array<int, 3> DAMAGE_VALUES_    = {15, 20, 30};    //ダメージ
    const float              EFFECT_DURATIONS_ = 5.0f;            //効果時間
    const float              EFFECT_INTERVAL_  = 0.25f;           //効果の間隔
    float                    effect_timer_     = 0.0f;            //効果時間タイマー
    float                    interval_timer_   = 0.0f;            //効果間隔タイマー
    float                    attack_range_     = 3.0f;            //効果範囲
};

CEREAL_REGISTER_TYPE(ChaturangaMantriSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChaturangaMantriSkill)
