//---------------------------------------------------------------------------
//!	@file	ChessPawnSkill.h
//! @brief	チェスポーンのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChessPawnSkill);
class Piece;    // 前方宣言
class ChessPawnSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChessPawnSkill, u8"チェスポーンのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChessPawnSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<float, 3> DAMAGE_RATES_ = {3.0f, 4.0f, 6.0f};    //ダメージ倍率
};

CEREAL_REGISTER_TYPE(ChessPawnSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChessPawnSkill)
