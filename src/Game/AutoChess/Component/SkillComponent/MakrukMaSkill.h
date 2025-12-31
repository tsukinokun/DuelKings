//---------------------------------------------------------------------------
//!	@file	MakrukMaSkill.h
//! @brief	マーのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(MakrukMaSkill);
class Piece;    // 前方宣言
class MakrukMaSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(MakrukMaSkill, u8"マーのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    MakrukMaSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3>   HEAL_AMOUNT_          = {200, 300, 400};       //回復量
    const std::array<int, 3>   PHYSICAL_DOWN_VALUES_ = {4, 7, 15};            //物理防御が下がる値
    const std::array<float, 3> UP_ATTACK_INTERVL_    = {0.3f, 0.4f, 0.5f};    //攻撃間隔の倍率
    const float                EFFECT_DURATION_      = 15.0f;                 //効果時間
    const float                AREA_RADIUS_          = 4.0f;                  //効果範囲の半径
};

CEREAL_REGISTER_TYPE(MakrukMaSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MakrukMaSkill)
