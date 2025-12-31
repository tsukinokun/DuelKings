//---------------------------------------------------------------------------
//!	@file	MakrukKhunSkill.h
//! @brief	クンのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(MakrukKhunSkill);
class Piece;    // 前方宣言
class MakrukKhunSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(MakrukKhunSkill, u8"クンのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    MakrukKhunSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3>   REMOVE_MP_VALUES_ = {50, 75, 100};          // MPを減らす値
    const std::array<float, 3> DAMAGE_RATE_      = {2.0f, 2.67f, 3.0f};    // MPからのダメージの割合
    const float                EFFECT_RANGE_     = 5.0f;                   // 効果範囲
};

CEREAL_REGISTER_TYPE(MakrukKhunSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MakrukKhunSkill)
