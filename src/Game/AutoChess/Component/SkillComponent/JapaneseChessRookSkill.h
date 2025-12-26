//---------------------------------------------------------------------------
//!	@file	JapaneseChessRookSkill.h
//! @brief	飛車のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(JapaneseChessRookSkill);
class Piece;    // 前方宣言
class JapaneseChessRookSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(JapaneseChessRookSkill, u8"飛車のスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    JapaneseChessRookSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3>   DAMAGE_VALUES_          = {300, 400, 600};          //ダメージ量
    const std::array<float, 3> ADDITIONAL_DAMAGE_RATE_ = {0.15f, 0.25f, 0.35f};    //追加ダメージ率
};

CEREAL_REGISTER_TYPE(JapaneseChessRookSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JapaneseChessRookSkill)
