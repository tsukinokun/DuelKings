//---------------------------------------------------------------------------
//!	@file	MakrukBiaSkill.h
//! @brief	ビアのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(MakrukBiaSkill);
class Piece;    // 前方宣言
class MakrukBiaSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(MakrukBiaSkill, u8"ビアのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    MakrukBiaSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> DAMAGE_VALUES_      = {100, 200, 400};    //ダメージ
    const float              KNOCKBACK_STRENGTH_ = 0.3f;               //ノックバック距離
    const float              KNOCKBACK_TIME_     = 0.4f;               //ノックバック時間
};

CEREAL_REGISTER_TYPE(MakrukBiaSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MakrukBiaSkill)
