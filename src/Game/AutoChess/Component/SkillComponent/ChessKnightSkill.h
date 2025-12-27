//---------------------------------------------------------------------------
//!	@file	ChessKnightSkill.h
//! @brief	チェスナイトのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChessKnightSkill);

class ChessKnightSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChessKnightSkill, u8"チェスナイトのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChessKnightSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const float              EFFECT_RADIUS_ = 2.0f;               //効果範囲
    const std::array<int, 3> DAMAGE_VALUES_ = {200, 400, 720};    //各レベルのダメージ量
    const std::array<int, 3> CURE_AMOUNT_   = {200, 400, 720};    //各レベルの回復量
};

CEREAL_REGISTER_TYPE(ChessKnightSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChessKnightSkill)
