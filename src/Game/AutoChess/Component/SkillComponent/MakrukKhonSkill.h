//---------------------------------------------------------------------------
//!	@file	MakrukKhonSkill.h
//! @brief	コーンのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(MakrukKhonSkill);
class Piece;    // 前方宣言
class MakrukKhonSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(MakrukKhonSkill, u8"コーンのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    MakrukKhonSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> PHYSICAL_DOWN_VALUES_ = {10, 20, 30};    //物理防御が下がる値
    const std::array<int, 3> DAMAGE_VALUES_        = {15, 30, 45};    //ダメージ量
    const float              EFFECT_DURATION_      = 5.0f;            //効果時間
};

CEREAL_REGISTER_TYPE(MakrukKhonSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MakrukKhonSkill)
