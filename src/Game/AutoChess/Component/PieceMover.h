//---------------------------------------------------------------------------
//!	@file	PieceMover.h
//! @brief	ピースの移動機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>

USING_PTR(PieceMover);

class PieceMover : public Component
{
public:
    BP_COMPONENT_DECL(PieceMover, u8"ピースの移動機能クラス");

    virtual void Init() override;    //!< 初期化
};

CEREAL_REGISTER_TYPE(PieceMover)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceMover)
