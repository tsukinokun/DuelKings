//---------------------------------------------------------------------------
//!	@file	MakrukRuaSkill.h
//! @brief	ルアのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(MakrukRuaSkill);
class Piece;    // 前方宣言
class MakrukRuaSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(MakrukRuaSkill, u8"ルアのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    MakrukRuaSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3>   TOTAL_ATTACKS_              = {10, 15, 25};          //攻撃回数
    const std::array<int, 3>   DAMAGE_VALUES_              = {175, 250, 350};       //ダメージ量
    const float                EFFECT_INTERVAL_            = 0.15f;                 //エフェクト発生間隔
    const float                ATTACK_INTERVAL_DOWN_VALUE_ = 0.25f;                 //攻撃間隔を遅らせる割合
    const std::array<float, 3> ATTACK_INTERVAL_DOWN_TIME_  = {3.0f, 4.0f, 5.0f};    //攻撃間隔を遅らせる時間
    int                        attack_count_               = 0;                     //残りの攻撃回数
    float                      attack_timer_               = 0.0f;                  //攻撃間隔タイマー
};

CEREAL_REGISTER_TYPE(MakrukRuaSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MakrukRuaSkill)
