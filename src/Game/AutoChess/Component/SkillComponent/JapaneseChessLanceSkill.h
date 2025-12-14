//---------------------------------------------------------------------------
//!	@file	JapaneseChessLanceSkill.h
//! @brief	香車のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(JapaneseChessLanceSkill);
class Piece;    // 前方宣言
class JapaneseChessLanceSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(JapaneseChessLanceSkill, u8"香車のスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    JapaneseChessLanceSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> DAMAGE_VALUES_      = {50, 75, 100};    //ダメージ量
    const float              DURATION_OF_EFFECT_ = 3.0f;             //効果時間
    float                    effect_timer_       = 0.0f;             //効果時間タイマー
    std::vector<Piece*>      hit_pieces_;                            //当たったピースのリスト
};

CEREAL_REGISTER_TYPE(JapaneseChessLanceSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JapaneseChessLanceSkill)
