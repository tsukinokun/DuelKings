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
    BP_COMPONENT_DECL(ChessKnightSkill, u8"チェスナイトのベースコンポーネントクラス");

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
};

CEREAL_REGISTER_TYPE(ChessKnightSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChessKnightSkill)
