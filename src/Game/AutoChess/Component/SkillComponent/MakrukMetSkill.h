//---------------------------------------------------------------------------
//!	@file	MakrukMetSkill.h
//! @brief	メットのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(MakrukMetSkill);
class Piece;    // 前方宣言
class MakrukMetSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(MakrukMetSkill, u8"メットのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    MakrukMetSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> PHYSICAL_DOWN_VALUES_ = {15, 30, 45};    //物理防御が下がる値
    const float              EFFECT_DURATION_      = 20.0f;           //効果時間
};

CEREAL_REGISTER_TYPE(MakrukMetSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MakrukMetSkill)
