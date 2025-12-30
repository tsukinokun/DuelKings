//---------------------------------------------------------------------------
//!	@file	ChaturangaGajaSkill.h
//! @brief	ガジャのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChaturangaGajaSkill);
class Piece;    // 前方宣言
class ChaturangaGajaSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChaturangaGajaSkill, u8"ガジャのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChaturangaGajaSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3>   DAMAGE_VALUES_  = {200, 400, 600};       //ダメージ
    const std::array<float, 3> STUN_DURATIONS_ = {2.0f, 3.0f, 4.0f};    //スタン時間
};

CEREAL_REGISTER_TYPE(ChaturangaGajaSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChaturangaGajaSkill)
