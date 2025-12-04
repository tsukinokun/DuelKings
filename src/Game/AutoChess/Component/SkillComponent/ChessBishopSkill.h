//---------------------------------------------------------------------------
//!	@file	ChessBishopSkill.h
//! @brief	チェスビショップのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChessBishopSkill);

class ChessBishopSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChessBishopSkill, u8"チェスビショップのベースコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChessBishopSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> HEAL_AMOUNT_ = {15, 30, 45};    //各レベルの回復量
};

CEREAL_REGISTER_TYPE(ChessBishopSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChessBishopSkill)
