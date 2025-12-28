//---------------------------------------------------------------------------
//!	@file	ChessKingSkill.h
//! @brief	チェスキングのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChessKingSkill);
class Piece;    // 前方宣言
class ChessKingSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChessKingSkill, u8"チェスキングのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChessKingSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const float                EFFECT_RADIUS_  = 2.5f;                  //効果範囲
    const std::array<float, 3> DAMAGE_VALUE_   = {200, 300, 500};       //ダメージ倍率
    const std::array<float, 3> STUN_DURATIONS_ = {1.0f, 1.5f, 2.0f};    //スタン時間
};

CEREAL_REGISTER_TYPE(ChessKingSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChessKingSkill)
