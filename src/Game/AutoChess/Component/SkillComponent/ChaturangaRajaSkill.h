//---------------------------------------------------------------------------
//!	@file	ChaturangaRajaSkill.h
//! @brief	ラジャのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChaturangaRajaSkill);
class Piece;    // 前方宣言
class ChaturangaRajaSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChaturangaRajaSkill, u8"ラジャのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChaturangaRajaSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> DEFAULT_DAMAGE_VALUES_  = {100, 150, 200};    //デフォルトダメージ
    const std::array<int, 3> INCREASE_DAMAGE_VALUES_ = {30, 45, 60};       //増加ダメージ
};

CEREAL_REGISTER_TYPE(ChaturangaRajaSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChaturangaRajaSkill)
