//---------------------------------------------------------------------------
//!	@file	JapaneseChessKingSkill.h
//! @brief	王将のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(JapaneseChessKingSkill);

class JapaneseChessKingSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(JapaneseChessKingSkill, u8"王将のスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    JapaneseChessKingSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const float              EFFECT_RADIUS_ = 3.0f;               //効果範囲の半径
    const std::array<int, 3> DAMAGE_VALUES_ = {100, 175, 250};    //各レベルのダメージ量
    const std::array<int, 3> HEAL_AMOUNT_   = {100, 150, 200};    //各レベルの回復量
};

CEREAL_REGISTER_TYPE(JapaneseChessKingSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JapaneseChessKingSkill)
