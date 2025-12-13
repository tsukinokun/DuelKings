//---------------------------------------------------------------------------
//!	@file	JapaneseChessLanceSkill.h
//! @brief	香車のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(JapaneseChessLanceSkill);

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
};

CEREAL_REGISTER_TYPE(JapaneseChessLanceSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JapaneseChessLanceSkill)
