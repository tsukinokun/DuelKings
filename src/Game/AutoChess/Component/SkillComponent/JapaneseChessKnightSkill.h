//---------------------------------------------------------------------------
//!	@file	JapaneseChessKnightSkill.h
//! @brief	桂馬のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(JapaneseChessKnightSkill);
class Piece;    // 前方宣言
class JapaneseChessKnightSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(JapaneseChessKnightSkill, u8"桂馬のスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    JapaneseChessKnightSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> DAMAGE_VALUES_ = {100, 150, 200};    //ダメージ量
};

CEREAL_REGISTER_TYPE(JapaneseChessKnightSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JapaneseChessKnightSkill)
