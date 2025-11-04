//---------------------------------------------------------------------------
//!	@file	PieceHPDisplayer.h
//! @brief	ピースのHP表示機能クラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once

#include <System/Component/Component.h>
class Piece;    //前方宣言
USING_PTR(PieceHPDisplayer);

class PieceHPDisplayer : public Component
{
public:
    BP_COMPONENT_DECL(PieceHPDisplayer, u8"ピースのHP表示機能クラス");

    virtual void Init() override;    //!< 初期化

private:
};

CEREAL_REGISTER_TYPE(PieceHPDisplayer)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PieceHPDisplayer)
