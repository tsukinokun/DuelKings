//---------------------------------------------------------------------------
//!	@file	JapaneseChessSilverSkill.h
//! @brief	銀将のスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(JapaneseChessSilverSkill);
class Piece;    // 前方宣言
class JapaneseChessSilverSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(JapaneseChessSilverSkill, u8"銀将のスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    JapaneseChessSilverSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> DAMAGE_VALUES_ = {400, 600, 800};    //ダメージ量
};

CEREAL_REGISTER_TYPE(JapaneseChessSilverSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, JapaneseChessSilverSkill)
