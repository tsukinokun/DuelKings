//---------------------------------------------------------------------------
//!	@file	ChessBishop.h
//! @brief	チェスのビショップクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChessBishop);
class ChessBishop : public Piece
{
public:
    BP_OBJECT_DECL(ChessBishop, u8"チェスのビショップクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
