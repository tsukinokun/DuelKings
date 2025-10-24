//---------------------------------------------------------------------------
//!	@file	ChessKing.h
//! @brief	チェスのキングクラス
//! @author 山﨑愛
//---------------------------------------------------------------------------
#pragma once
#include <System/Scene.h>
#include "Piece.h"
USING_PTR(ChessKing);
class ChessKing : public Piece
{
public:
    BP_OBJECT_DECL(ChessKing, u8"チェスのキングクラス")
    //@{
    bool Init() override;    //!< 初期化
    //@}
};
