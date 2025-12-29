//---------------------------------------------------------------------------
//!	@file	ChaturangaPadatiSkill.h
//! @brief	パダチのスキルコンポーネントクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
#include <Game/AutoChess/Component/SkillComponent/ComponentActiveSkill.h>

USING_PTR(ChaturangaPadatiSkill);
class Piece;    // 前方宣言
class ChaturangaPadatiSkill : public ComponentActiveSkill
{
public:
    BP_COMPONENT_DECL(ChaturangaPadatiSkill, u8"パダチのスキルコンポーネントクラス");
    //---------------------------------------------------------
    // コンストラクタ
    //---------------------------------------------------------
    ChaturangaPadatiSkill();

    void Init() override;    //!< 初期化

    //---------------------------------------------------------
    // スキル発動処理
    //---------------------------------------------------------
    void Activate() override;

private:
    const std::array<int, 3> DAMAGE_VALUES_ = {200, 400, 600};    //ダメージ倍率
    const float              STUN_DURATION_ = 0.1f;               //スタン時間
};

CEREAL_REGISTER_TYPE(ChaturangaPadatiSkill)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, ChaturangaPadatiSkill)
