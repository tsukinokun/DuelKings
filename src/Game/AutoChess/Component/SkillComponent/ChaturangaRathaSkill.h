//---------------------------------------------------------------------------
//!	@file	ChaturangaRathaSkill.h
//! @brief	ラタのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChaturangaRathaSkill);
class Piece;    // 前方宣言
class ChaturangaRathaSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChaturangaRathaSkill, u8"ラタのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChaturangaRathaSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3>   DAMAGE_VALUES_  = {100, 200, 300};       //ダメージ
    const std::array<float, 3> STUN_DURATIONS_ = {2.0f, 2.5f, 3.5f};    //スタン時間
    const std::array<float, 3> EFFECT_RANGES_  = {3.0f, 5.0f, 6.0f};    //効果範囲
};

CEREAL_REGISTER_TYPE(ChaturangaRathaSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChaturangaRathaSkill)
