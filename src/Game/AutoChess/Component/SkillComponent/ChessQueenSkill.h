//---------------------------------------------------------------------------
//!	@file	ChessQueenSkill.h
//! @brief	チェスクイーンのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChessQueenSkill);

class ChessQueenSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChessQueenSkill, u8"チェスクイーンのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChessQueenSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<float, 3> DAMAGE_RATE_ = {0.04, 0.06, 0.08};    //各レベルのダメージ量
};

CEREAL_REGISTER_TYPE(ChessQueenSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChessQueenSkill)
