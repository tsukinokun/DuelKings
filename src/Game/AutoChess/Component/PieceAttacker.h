//---------------------------------------------------------------------------
//!	@file	PieceAttacker.h
//! @brief	ピースの攻撃機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>

USING_PTR(PieceAttacker);

class PieceAttacker : public Component
{
public:
    BP_COMPONENT_DECL(PieceAttacker, u8"ピースの攻撃機能クラス");

    virtual void Init() override;    //!< 初期化
private:
    float attack_timer_ = 0.0f;
};

CEREAL_REGISTER_TYPE(PieceAttacker)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceAttacker)
